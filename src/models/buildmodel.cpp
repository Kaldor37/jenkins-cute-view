//------------------------------------------------------------------------------
#include "models/buildmodel.h"
#include "utils/httpgetter.h"
#include "preferences.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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
BuildModel::BuildModel(uint number, const QString &url, QObject *parent):QObject(parent),
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

	HttpGetter::instance().get(m_Url + "/api/json", this, &BuildModel::http_finished);
}
//------------------------------------------------------------------------------
void BuildModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"BuildModel["<<m_Number<<"]::http_finished("<<errCode<<") : "<<error;
		return;
	}

	QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
	Q_ASSERT(jsonDoc.isObject());
	const QJsonObject rootObj = jsonDoc.object();

	setBuilding(rootObj["building"].toBool());
	setDescription(rootObj["description"].toString());
	setFullDisplayName(rootObj["fullDisplayName"].toString());
	setId(rootObj["id"].toString());
	setNumber(static_cast<uint>(rootObj["id"].toInt()));
	setResult(rootObj["result"].toString());
	setUrl(rootObj["url"].toString());
	setBuiltOn(rootObj["builtOn"].toString());
	setDuration(rootObj["duration"].toVariant().toULongLong());
	setEstimatedDuration(rootObj["estimatedDuration"].toVariant().toULongLong());
	setTimestamp(rootObj["timestamp"].toVariant().toULongLong());

	m_loaded = true;
	emit loaded();
}
//------------------------------------------------------------------------------
