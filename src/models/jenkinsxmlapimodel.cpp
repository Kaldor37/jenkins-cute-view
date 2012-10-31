//------------------------------------------------------------------------------
#include "jenkinsxmlapimodel.h"
#include "utils/httpgetter.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomNodeList>
#include <QStringList>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JenkinsXMLAPIModel::JenkinsXMLAPIModel(QObject *parent/*=0*/): QObject(parent),
	m_jenkinsUrl(),
	m_viewsListLoaded(false),
	m_views(),
	m_primaryView(0),
	m_selectedView(0){
}
//------------------------------------------------------------------------------
JenkinsXMLAPIModel::~JenkinsXMLAPIModel(){
	clearViews();
}
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
bool JenkinsXMLAPIModel::isViewsListLoaded() const {
	return m_viewsListLoaded;
}
//------------------------------------------------------------------------------
const JenkinsXMLAPIModel::ViewsList & JenkinsXMLAPIModel::views() const {
	return m_views;
}
//------------------------------------------------------------------------------
const ViewModel * JenkinsXMLAPIModel::primaryView() const {
	return m_primaryView;
}
//------------------------------------------------------------------------------
const ViewModel * JenkinsXMLAPIModel::selectedView() const {
	return m_selectedView;
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::setJenkinsUrl(const QString &url){
	if(m_jenkinsUrl != url){
		// Trim additionnal right '/'
		QString trimmed = url;
		while(trimmed.endsWith('/'))
			trimmed = trimmed.left(url.length()-1);

		m_jenkinsUrl = trimmed;

		if(!m_jenkinsUrl.isEmpty())
			loadViews();
	}
}
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::setSelectedView(const QString &name){
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
void JenkinsXMLAPIModel::loadViews(){
	if(m_jenkinsUrl.isEmpty())
		return;

	m_viewsListLoaded = false;

	httpGetter.get(m_jenkinsUrl + "/api/xml", this, SLOT(http_finished(QString,QNetworkReply::NetworkError,QString)));
}
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::loadSelectedView(){
	if(!m_selectedView)
		return;

	connect(m_selectedView, SIGNAL(jobsListLoaded()), SLOT(selectedView_jobsListLoaded()));
	m_selectedView->loadJobs();
}
//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"JenkinsXMLAPIModel::httpGetter_finished - Error : "<<errCode<<" ("<<error<<")";
		return;
	}

	QDomDocument doc;
	bool parsed = doc.setContent(content);
	if(!parsed){
		qWarning()<<"JenkinsXMLAPIModel::httpGetter_finished - Error parsing XML !";
		return;
	}

	parseViews(doc);
}
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::selectedView_jobsListLoaded(){
	m_viewsListLoaded = true;

	ViewModel * loadedView = qobject_cast<ViewModel*>(QObject::sender());
	if(loadedView && m_selectedView && m_selectedView == loadedView){
		emit selectedViewLoaded();
	}
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::parseViews(const QDomDocument &doc){
	if(m_selectedView)
		m_selectedView->disconnect(this);

	m_selectedView = 0;
	m_primaryView = 0;

	clearViews();

	QString primaryViewName;
	QDomNodeList primaryViewsList = doc.elementsByTagName("primaryView");
	if(primaryViewsList.length() <= 0){
		qWarning()<<"JenkinsXMLAPIModel::parseViews - No primary view !";
		return;
	}
	QDomElement primaryViewNameElm = primaryViewsList.at(0).firstChildElement("name");
	if(!primaryViewNameElm.isNull()){
		primaryViewName = primaryViewNameElm.text();
	}

	// Loads views list
	QDomNodeList viewsList = doc.elementsByTagName("view");
	int nbViews = viewsList.length();
	if(nbViews <= 0){
		qWarning()<<"JenkinsXMLAPIModel::parseViews - No view found !";
		return;
	}

	QStringList viewsNamesList;
	m_views.reserve(nbViews);

	for(int i=0 ; i < nbViews ; ++i){
		QDomNode viewNode = viewsList.at(i);
		QDomElement nameElm = viewNode.firstChildElement("name");
		QDomElement urlElm = viewNode.firstChildElement("url");

		if(!nameElm.isNull() && !urlElm.isNull()){
			QString viewName = nameElm.text();

			if(!viewName.isEmpty()){
				ViewModel *vm = new ViewModel(viewName, urlElm.text(), this);
				m_views.push_back(vm);

				// Pointer on selected view
				if(m_selectedViewName == viewName)
					m_selectedView = vm;
				// Pointer on primary view
				if(primaryViewName == viewName)
					m_primaryView = vm;

				viewsNamesList.append(viewName);
			}
		}
	}

	Q_ASSERT(m_primaryView);

	// If no selected view, selected becomes primary
	if(!m_selectedView && m_primaryView)
		m_selectedView = m_primaryView;

	qDebug()<<"JenkinsXMLAPIModel::parseViews - Parsed "<<m_views.size()<<" views - Primary : "<<m_primaryView->getName()<<" - Selected : "<<m_selectedView->getName();

	loadSelectedView();

	viewsNamesList.sort();
	emit viewsNamesUpdated(viewsNamesList, m_primaryView->getName());
}
//------------------------------------------------------------------------------
void JenkinsXMLAPIModel::clearViews(){
	foreach(ViewModel *view, m_views)
		view->deleteLater();

	m_views.clear();
}
//------------------------------------------------------------------------------
