//------------------------------------------------------------------------------
#include "jobmodel.h"
#include "models/buildmodel.h"
#include "utils/httpgetter.h"
#include "preferences.h"

#include <QDomDocument>
#include <QDebug>
//------------------------------------------------------------------------------
#define JOB_INIT_INT_MEMBER(Member) m_##Member(0)
#define JOB_INIT_STR_MEMBER(Member) m_##Member()

#define JOB_IMPL_MEMBER(Type, Member) \
	const Type & JobModel::get##Member() const { \
		return m_##Member; \
	} \
	void JobModel::set##Member(const Type & value){ \
		m_##Member = value; \
	}
//------------------------------------------------------------------------------
JobModel::JobModel(const QString &name, const QString &url, QObject *parent/*=0*/):QObject(parent),
	m_lastBuildLoaded(false),
	m_lastCompletedBuildLoaded(false),
	m_Name(name),
	m_Url(url),
	JOB_INIT_STR_MEMBER(Description),
	JOB_INIT_STR_MEMBER(DisplayName),
	JOB_INIT_INT_MEMBER(Buildable),
	JOB_INIT_STR_MEMBER(Color),
	JOB_INIT_INT_MEMBER(InQueue),
	JOB_INIT_INT_MEMBER(KeepDependencies),
	JOB_INIT_STR_MEMBER(HealthDescription),
	JOB_INIT_INT_MEMBER(HealthScore),
	JOB_INIT_INT_MEMBER(NextBuildNumber),
	JOB_INIT_INT_MEMBER(ConcurrentBuild)
{}
//------------------------------------------------------------------------------
JobModel::~JobModel(){
	if(m_lastBuild)
		m_lastBuild->deleteLater();

	if(m_lastCompletedBuild)
		m_lastCompletedBuild->deleteLater();
}
//------------------------------------------------------------------------------
JOB_IMPL_MEMBER(QString,	Name)
JOB_IMPL_MEMBER(QString,	Url)
JOB_IMPL_MEMBER(QString,	Description)
JOB_IMPL_MEMBER(QString,	DisplayName)
JOB_IMPL_MEMBER(bool,		Buildable)
JOB_IMPL_MEMBER(QString,	Color)
JOB_IMPL_MEMBER(bool,		InQueue)
JOB_IMPL_MEMBER(bool,		KeepDependencies)
JOB_IMPL_MEMBER(QString,	HealthDescription)
JOB_IMPL_MEMBER(int,			HealthScore)
JOB_IMPL_MEMBER(uint,		NextBuildNumber)
JOB_IMPL_MEMBER(bool,		ConcurrentBuild)
//------------------------------------------------------------------------------
#undef JOB_INIT_INT_MEMBER
#undef JOB_INIT_STR_MEMBER
#undef JOB_IMPL_MEMBER
//------------------------------------------------------------------------------
bool JobModel::isLoaded() const{
	return (m_lastBuildLoaded && m_lastCompletedBuildLoaded);
}
//------------------------------------------------------------------------------
const BuildModel * JobModel::getLastBuild() const {
	return m_lastBuild;
}
//------------------------------------------------------------------------------
const BuildModel * JobModel::getLastCompletedBuild() const {
	return m_lastCompletedBuild;
}
//------------------------------------------------------------------------------
void JobModel::load(){
	if(m_Url.isEmpty())
		return;

	m_lastBuildLoaded = m_lastCompletedBuildLoaded = false;

	httpGetter.get(m_Url + "api/xml", this, SLOT(http_finished(QString,QNetworkReply::NetworkError,QString)));
}
//------------------------------------------------------------------------------
void JobModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"JobModel["<<m_Name<<"]::http_finished("<<errCode<<") : "<<error;
		return;
	}

	QDomDocument doc;
	bool parsed = doc.setContent(content);
	if(!parsed){
		qWarning()<<"JobModel["<<m_Name<<"]::http_finished - Error parsing XML !";
		return;
	}

	parseJob(doc);
}
//------------------------------------------------------------------------------
void JobModel::parseJob(const QDomDocument &doc){
	QDomNodeList ndList;
	QDomElement elm;

	if((ndList = doc.elementsByTagName("description")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setDescription(elm.text());

	if((ndList = doc.elementsByTagName("displayName")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setDisplayName(elm.text());

	if((ndList = doc.elementsByTagName("buildable")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setBuildable(elm.text() == "true");

	if((ndList = doc.elementsByTagName("color")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setColor(elm.text());

	if((ndList = doc.elementsByTagName("inQueue")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setInQueue(elm.text() == "true");

	if((ndList = doc.elementsByTagName("keepDependencies")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setKeepDependencies(elm.text() == "true");

	if((ndList = doc.elementsByTagName("healthReport")).count() > 0){
		if(!((elm = ndList.at(0).firstChildElement("description")).isNull()))
			setHealthDescription(elm.text());
		if(!((elm = ndList.at(0).firstChildElement("score")).isNull()))
			setHealthScore(elm.text().toInt());
	}

	if((ndList = doc.elementsByTagName("nextBuildNumber")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setNextBuildNumber(elm.text().toUInt());

	if((ndList = doc.elementsByTagName("concurrentBuild")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setConcurrentBuild(elm.text() == "true");

	if(m_lastBuild){
		m_lastBuild->deleteLater();
		m_lastBuild = nullptr;
	}

	if(m_lastCompletedBuild){
		m_lastCompletedBuild->deleteLater();
		m_lastCompletedBuild = nullptr;
	}

	// Load last build
	if((ndList = doc.elementsByTagName("lastBuild")).count() > 0){
		uint lastBuildNumber = 0;
		QString lastBuildUrl;

		if(!((elm = ndList.at(0).firstChildElement("number")).isNull()))
			lastBuildNumber = elm.text().toUInt();
		if(!((elm = ndList.at(0).firstChildElement("url")).isNull()))
			lastBuildUrl = elm.text();

		m_lastBuild = new BuildModel(lastBuildNumber, lastBuildUrl, this);
		QObject::connect(m_lastBuild, &BuildModel::loaded, this, &JobModel::lastBuild_loaded);
		m_lastBuild->load();
	}
	// No last build, consider it loaded
	else
		emit lastBuild_loaded();

	// Load last completed build
	if((ndList = doc.elementsByTagName("lastCompletedBuild")).count() > 0){
		uint lastBuildNumber = 0;
		QString lastBuildUrl;

		if(!((elm = ndList.at(0).firstChildElement("number")).isNull()))
			lastBuildNumber = elm.text().toUInt();
		if(!((elm = ndList.at(0).firstChildElement("url")).isNull()))
			lastBuildUrl = elm.text();

		m_lastCompletedBuild = new BuildModel(lastBuildNumber, lastBuildUrl, this);
		QObject::connect(m_lastCompletedBuild, &BuildModel::loaded, this, &JobModel::lastCompletedBuild_loaded);
		m_lastCompletedBuild->load();
	}
	// No last completed build, consider it loaded
	else
		emit lastCompletedBuild_loaded();
}
//------------------------------------------------------------------------------
void JobModel::lastBuild_loaded(){
	m_lastBuildLoaded = true;

	if(m_lastCompletedBuildLoaded)
		emit loaded();
}
//------------------------------------------------------------------------------
void JobModel::lastCompletedBuild_loaded(){
	m_lastCompletedBuildLoaded = true;

	if(m_lastBuildLoaded)
		emit loaded();
}
//------------------------------------------------------------------------------
