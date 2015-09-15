//------------------------------------------------------------------------------
#include "jenkinsapimodel.h"
#include "utils/httpgetter.h"
#include "models/viewmodel.h"
#include "preferences.h"
#include "application.h"

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JenkinsAPIModel::JenkinsAPIModel(QObject *parent): QObject(parent),
	m_viewsListLoaded(false),
	m_primaryView(nullptr),
	m_selectedView(nullptr)
{
}
//------------------------------------------------------------------------------
JenkinsAPIModel::~JenkinsAPIModel(){
	clearViews();
}
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
bool JenkinsAPIModel::isViewsListLoaded() const {
	return m_viewsListLoaded;
}
//------------------------------------------------------------------------------
const JenkinsAPIModel::ViewsList & JenkinsAPIModel::views() const {
	return m_views;
}
//------------------------------------------------------------------------------
const ViewModel * JenkinsAPIModel::primaryView() const {
	return m_primaryView;
}
//------------------------------------------------------------------------------
const ViewModel * JenkinsAPIModel::selectedView() const {
	return m_selectedView;
}
//------------------------------------------------------------------------------
const QVector<QString> & JenkinsAPIModel::jobsQueue() const {
	return m_jobsQueue;
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JenkinsAPIModel::setJenkinsUrl(const QString &url){
	if(m_jenkinsUrl != url){
		// Trim additionnal right '/'
		QString trimmed = url;
		while(trimmed.endsWith('/'))
			trimmed = trimmed.left(url.length()-1);

		m_jenkinsUrl = trimmed;

		if(!m_jenkinsUrl.isEmpty()){
			emit message(tr("Loading..."));
			loadViews();
			loadJobsQueue();
		}
		else
			emit message(tr("Please set jenkins URL"));
	}
}
//------------------------------------------------------------------------------
void JenkinsAPIModel::setSelectedView(const QString &name){
	// Selected view has changed
	if(m_selectedViewName != name){
		m_selectedViewName = name;

		// Views list was loaded
		if(m_viewsListLoaded){
			// No view selected, use primary
			if(m_selectedViewName.isEmpty()){
				Q_ASSERT(m_primaryView);
				m_selectedView = m_primaryView;
			}
			// Search selected view in list
			else{
				for(int i = 0 ; i < m_views.size() ; i++){
					ViewModel *view = m_views.at(i);
					if(view->getName() == m_selectedViewName){
						m_selectedView = view;
						break;
					}
				}
			}
			Q_ASSERT(m_selectedView);
			loadSelectedView();
		}
	}
}
//------------------------------------------------------------------------------
void JenkinsAPIModel::loadViews(){
	if(m_jenkinsUrl.isEmpty()){
		emit message(tr("Please set jenkins URL"));
		return;
	}

	m_viewsListLoaded = false;

	HttpGetter::instance().get(m_jenkinsUrl + "/api/json", this, &JenkinsAPIModel::viewsList_httpFinished);
}
//------------------------------------------------------------------------------
void JenkinsAPIModel::loadSelectedView(){
	if(!m_selectedView)
		return;

	QObject::connect(m_selectedView, &ViewModel::jobsListLoaded, this, &JenkinsAPIModel::selectedView_jobsListLoaded);
	m_selectedView->loadJobs();
}
//------------------------------------------------------------------------------
void JenkinsAPIModel::loadJobsQueue(){
	if(m_jenkinsUrl.isEmpty())
		return;

	HttpGetter::instance().get(m_jenkinsUrl + "/queue/api/json", this, &JenkinsAPIModel::jobsQueue_httpFinished);
}
//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
void JenkinsAPIModel::viewsList_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"JenkinsAPIModel::viewsList_httpFinished - Error : "<<errCode<<" ("<<error<<")";
		emit this->error(error);
		return;
	}

	if(m_selectedView)
		m_selectedView->disconnect(this);

	m_selectedView = nullptr;
	m_primaryView = nullptr;

	QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
	Q_ASSERT(jsonDoc.isObject());
	const QJsonObject rootObj = jsonDoc.object();

	Q_ASSERT(rootObj.contains("primaryView"));
	Q_ASSERT(rootObj["primaryView"].isObject());
	const QString primaryViewName = rootObj["primaryView"].toObject()["name"].toString();
	Q_ASSERT(!primaryViewName.isEmpty());

	Q_ASSERT(rootObj.contains("views"));
	Q_ASSERT(rootObj["views"].isArray());
	const QJsonArray viewsArray = rootObj["views"].toArray();
	if(viewsArray.size() == 0){
		emit this->error(tr("No view found"));
		qWarning()<<"JenkinsAPIModel::parseViews - No view found !";
		return;
	}

	QStringList viewNames;
	viewNames.reserve(viewsArray.size());

	for(const QJsonValue jVal : viewsArray){
		Q_ASSERT(jVal.isObject());
		QJsonObject viewObj = jVal.toObject();
		Q_ASSERT(viewObj["name"].isString());
		Q_ASSERT(viewObj["url"].isString());
		const QString viewName = viewObj["name"].toString();
		Q_ASSERT(!viewName.isEmpty());

		ViewModel *model = nullptr;
		for(ViewModel *vm : m_views){
			if(vm->getName() == viewName){
				model = vm;
				model->setUrl(viewObj["url"].toString());
				break;
			}
		}
		if(!model){
			model = new ViewModel(viewName, viewObj["url"].toString(), this);
			m_views.push_back(model);
		}

		if(m_selectedViewName == viewName)
			m_selectedView = model;
		if(primaryViewName == viewName)
			m_primaryView = model;

		viewNames.append(viewName);
	}

	for(auto it = m_views.begin() ; it != m_views.end() ; ){
		ViewModel * vm = *it;
		if(!viewNames.contains(vm->getName())){
			if(m_selectedView == vm)
				m_selectedView = nullptr;
			vm->deleteLater();
			it = m_views.erase(it);
		}
		else
			++it;
	}

	Q_ASSERT(m_primaryView);

	// If no selected view, selected becomes primary
	if(!m_selectedView && m_primaryView)
		m_selectedView = m_primaryView;

	if(App.verbose())
		qDebug()<<"JenkinsAPIModel::parseViews - Parsed "<<m_views.size()<<" views - Primary : "<<m_primaryView->getName()<<" - Selected : "<<m_selectedView->getName();

	loadSelectedView();

	viewNames.sort();
	emit viewsNamesUpdated(viewNames, m_primaryView->getName());

}
//------------------------------------------------------------------------------
void JenkinsAPIModel::jobsQueue_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"JenkinsAPIModel::jobsQueue_httpFinished - Error : "<<errCode<<" ("<<error<<")";
		emit this->error(error);
		return;
	}

	QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
	Q_ASSERT(jsonDoc.isObject());
	QJsonObject main = jsonDoc.object();
	Q_ASSERT(main.contains("items"));
	QJsonArray items = main["items"].toArray();

	QVector<QString> jobsQueue;
	jobsQueue.reserve(items.size());

	for(const QJsonValueRef &jItem : items){
		Q_ASSERT(jItem.isObject());
		const QJsonObject oItem = jItem.toObject();
		Q_ASSERT(oItem.contains("task"));
		const QJsonObject oTask = oItem["task"].toObject();
		Q_ASSERT(oTask.contains("name"));
		const QString jobName = oTask["name"].toString();
		if(!jobName.isEmpty())
			jobsQueue.push_front(jobName);
	}

	m_jobsQueue = std::move(jobsQueue);
	emit jobsQueueLoaded();
}
//------------------------------------------------------------------------------
void JenkinsAPIModel::selectedView_jobsListLoaded(){
	m_viewsListLoaded = true;

	ViewModel * loadedView = qobject_cast<ViewModel*>(QObject::sender());
	if(loadedView && m_selectedView && m_selectedView == loadedView){
		emit selectedViewLoaded();
	}
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void JenkinsAPIModel::clearViews(){
	for(ViewModel *view : m_views)
		view->deleteLater();

	m_views.clear();
}
//------------------------------------------------------------------------------
