//------------------------------------------------------------------------------
#include "jobmodel.h"
#include "models/buildmodel.h"
#include "utils/httpgetter.h"
#include "preferences.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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
JobModel::JobModel(const QString &name, const QString &url, QObject *parent):QObject(parent),
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

	HttpGetter::instance().get(m_Url + "/api/json", this, &JobModel::http_finished);
}
//------------------------------------------------------------------------------
void JobModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"JobModel["<<m_Name<<"]::http_finished("<<errCode<<") : "<<error;
		return;
	}

	QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
	Q_ASSERT(jsonDoc.isObject());
	const QJsonObject rootObj = jsonDoc.object();

	setDescription(rootObj["description"].toString());
	setDisplayName(rootObj["displayName"].toString());
	setBuildable(rootObj["buildable"].toBool());
	setColor(rootObj["color"].toString());
	setInQueue(rootObj["inQueue"].toBool());
	setKeepDependencies(rootObj["keepDependencies"].toBool());
	setNextBuildNumber(static_cast<uint>(rootObj["nextBuildNumber"].toInt()));
	setConcurrentBuild(rootObj["concurrentBuild"].toBool());

	if(rootObj["healthReport"].isArray()){
		const QJsonArray healthReport = rootObj["healthReport"].toArray();
		if(healthReport.size() > 0 && healthReport.at(0).isObject()){
			const QJsonObject firstHealReport = healthReport.at(0).toObject();
			if(!firstHealReport["description"].isNull())
				setHealthDescription(firstHealReport["description"].toString());
			if(!firstHealReport["score"].isNull())
				setHealthScore(firstHealReport["score"].toInt());
		}
	}

	int lastBuildNumber = 0;
	QString lastBuildUrl;
	int lastCompletedBuildNumber = 0;
	QString lastCompletedBuildUrl;

	if(!rootObj["lastBuild"].isNull() && rootObj["lastBuild"].isObject()){
		const QJsonObject lastBuild = rootObj["lastBuild"].toObject();
		lastBuildNumber = lastBuild["number"].toInt();
		lastBuildUrl = lastBuild["url"].toString();
	}
	if(!rootObj["lastCompletedBuild"].isNull() && rootObj["lastCompletedBuild"].isObject()){
		const QJsonObject lastCompletedBuild = rootObj["lastCompletedBuild"].toObject();
		lastCompletedBuildNumber = lastCompletedBuild["number"].toInt();
		lastCompletedBuildUrl = lastCompletedBuild["url"].toString();
	}

	if(lastBuildNumber > 0 && !lastBuildUrl.isEmpty()){
		if(m_lastBuild){
			m_lastBuild->setNumber(lastBuildNumber);
			m_lastBuild->setUrl(lastBuildUrl);
		}
		else{
			m_lastBuild = new BuildModel(lastBuildNumber, lastBuildUrl, this);
			QObject::connect(m_lastBuild, &BuildModel::loaded, this, &JobModel::lastBuild_loaded);
		}
		m_lastBuild->load();
	}
	else{
		if(m_lastBuild){
			m_lastBuild->disconnect(this);
			m_lastBuild->deleteLater();
			m_lastBuild = nullptr;
		}
		lastBuild_loaded();
	}

	if(lastCompletedBuildNumber > 0 && !lastCompletedBuildUrl.isEmpty()){
		if(m_lastCompletedBuild){
			m_lastCompletedBuild->setNumber(lastCompletedBuildNumber);
			m_lastCompletedBuild->setUrl(lastCompletedBuildUrl);
		}
		else{
			m_lastCompletedBuild = new BuildModel(lastCompletedBuildNumber, lastCompletedBuildUrl, this);
			QObject::connect(m_lastCompletedBuild, &BuildModel::loaded, this, &JobModel::lastCompletedBuild_loaded);
		}
		m_lastCompletedBuild->load();
	}
	else{
		if(m_lastCompletedBuild){
			m_lastCompletedBuild->disconnect(this);
			m_lastCompletedBuild->deleteLater();
			m_lastCompletedBuild = nullptr;
		}
		lastCompletedBuild_loaded();
	}
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
