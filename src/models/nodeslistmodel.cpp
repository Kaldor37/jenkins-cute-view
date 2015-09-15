//------------------------------------------------------------------------------
#include "nodeslistmodel.h"

#include "utils/httpgetter.h"
#include "preferences.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
//------------------------------------------------------------------------------
NodesListModel::NodesListModel(QObject *parent) : QObject(parent){
}
//------------------------------------------------------------------------------
NodesListModel::~NodesListModel(){}
//------------------------------------------------------------------------------
void NodesListModel::update(){
	if(m_updating)
		return;

	m_updating = true;
	Q_ASSERT(!Prefs.getJenkinsUrl().isEmpty());
	QUrl url(Prefs.getJenkinsUrl() + "/computer/api/json");
	Q_ASSERT(url.isValid());
	HttpGetter::instance().get(url, this, &NodesListModel::httpFinished);
}
//------------------------------------------------------------------------------
void NodesListModel::httpFinished(const QString & data, QNetworkReply::NetworkError errorCode, const QString & errString){
	Q_UNUSED(errString);
	if(errorCode == QNetworkReply::NoError){
		QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
		Q_ASSERT(jsonDoc.isObject());
		QJsonObject main = jsonDoc.object();
		Q_ASSERT(main.contains("computer"));
		QJsonArray computers = main["computer"].toArray();

		NodesList nodes;
		nodes.reserve(computers.size());
		for(const QJsonValueRef &jVal : computers){
			Q_ASSERT(jVal.isObject());
			QJsonObject jNode = jVal.toObject();
			if(jNode["numExecutors"].toInt() <= 0)
				continue;

			Node node;
			node.displayName			= jNode["displayName"].toString();
			node.idle					= jNode["idle"].toBool();
			node.numExecutors			= jNode["numExecutors"].toInt();
			node.offline				= jNode["offline"].toBool();
			node.offlineCauseReason	= jNode["offlineCauseReason"].toString();
			node.temporarilyOffline = jNode["temporarilyOffline"].toBool();

			if(jNode["monitorData"].isObject()){
				QJsonObject monitorData = jNode["monitorData"].toObject();
				if(monitorData["hudson.node_monitors.ResponseTimeMonitor"].isObject()){
					node.reponseTime = monitorData["hudson.node_monitors.ResponseTimeMonitor"].toObject()["average"].toInt();
				}
			}

			nodes.push_back(node);
		}
		m_nodesList.swap(nodes);
	}
	emit updated();
	m_updating = false;
}
//------------------------------------------------------------------------------
