//------------------------------------------------------------------------------
#include "models/buildmodel.h"
#include "utils/httpgetter.h"
#include "preferences.h"

#include <QDebug>
#include <QDomDocument>
//------------------------------------------------------------------------------
#define BUILD_INIT_INT_MEMBER(Member) m_##Member(0)
#define BUILD_INIT_STR_MEMBER(Member) m_##Member()

#define BUILD_IMPL_MEMBER(Type, Member) \
	const Type & BuildModel::get##Member() const { \
		return m_##Member; \
	} \
	void BuildModel::set##Member(const Type & value){ \
		m_##Member = value; \
	}
//------------------------------------------------------------------------------
BuildModel::BuildModel(uint number, const QString &url, QObject *parent/*=0*/):QObject(parent),
	m_loaded(false),
	m_Number(number),
	m_Url(url),
	BUILD_INIT_INT_MEMBER(Building),
	BUILD_INIT_STR_MEMBER(Description),
	BUILD_INIT_INT_MEMBER(Duration),
	BUILD_INIT_INT_MEMBER(EstimatedDuration),
	BUILD_INIT_STR_MEMBER(FullDisplayName),
	BUILD_INIT_STR_MEMBER(Id),
	BUILD_INIT_STR_MEMBER(Result),
	BUILD_INIT_INT_MEMBER(Timestamp),
	BUILD_INIT_STR_MEMBER(BuiltOn)
{}
//------------------------------------------------------------------------------
BuildModel::~BuildModel(){}
//------------------------------------------------------------------------------
BUILD_IMPL_MEMBER(bool,			Building)
BUILD_IMPL_MEMBER(QString,		Description)
BUILD_IMPL_MEMBER(qint64,		Duration)
BUILD_IMPL_MEMBER(qint64,		EstimatedDuration)
BUILD_IMPL_MEMBER(QString,		FullDisplayName)
BUILD_IMPL_MEMBER(QString,		Id)
BUILD_IMPL_MEMBER(uint,			Number)
BUILD_IMPL_MEMBER(QString,		Result)
BUILD_IMPL_MEMBER(qint64,		Timestamp)
BUILD_IMPL_MEMBER(QString,		Url)
BUILD_IMPL_MEMBER(QString,		BuiltOn)
//------------------------------------------------------------------------------
#undef BUILD_INIT_INT_MEMBER
#undef BUILD_INIT_STR_MEMBER
#undef BUILD_IMPL_MEMBER
//------------------------------------------------------------------------------
bool BuildModel::isLoaded() const{
	return m_loaded;
}
//------------------------------------------------------------------------------
void BuildModel::load(){
	if(m_Url.isEmpty())
		return;

	m_loaded = false;

	httpGetter.get(m_Url + "api/xml", this, SLOT(http_finished(QString,QNetworkReply::NetworkError,QString)));
}
//------------------------------------------------------------------------------
void BuildModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"BuildModel["<<m_Number<<"]::http_finished("<<errCode<<") : "<<error;
		return;
	}

	QDomDocument doc;
	bool parsed = doc.setContent(content);
	if(!parsed){
		qWarning()<<"BuildModel["<<m_Number<<"]::http_finished - Error parsing XML !";
		return;
	}

	parseBuild(doc);
}
//------------------------------------------------------------------------------
void BuildModel::parseBuild(const QDomDocument &doc){
	QDomNodeList ndList;
	QDomElement elm;

	if((ndList = doc.elementsByTagName("building")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setBuilding(elm.text() == "true");

	if((ndList = doc.elementsByTagName("description")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setDescription(elm.text());

	if((ndList = doc.elementsByTagName("duration")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setDuration(elm.text().toLongLong());

	if((ndList = doc.elementsByTagName("estimatedDuration")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setEstimatedDuration(elm.text().toLongLong());

	if((ndList = doc.elementsByTagName("fullDisplayName")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setFullDisplayName(elm.text());

	if((ndList = doc.elementsByTagName("id")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setId(elm.text());

	if((ndList = doc.elementsByTagName("number")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setNumber(elm.text().toUInt());

	if((ndList = doc.elementsByTagName("result")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setResult(elm.text());

	if((ndList = doc.elementsByTagName("timestamp")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setTimestamp(elm.text().toLongLong());

	if((ndList = doc.elementsByTagName("url")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setUrl(elm.text());

	if((ndList = doc.elementsByTagName("builtOn")).count() > 0)
		if(!((elm = ndList.at(0).toElement()).isNull()))
			setBuiltOn(elm.text());

	m_loaded = true;

	emit loaded();
}
//------------------------------------------------------------------------------
