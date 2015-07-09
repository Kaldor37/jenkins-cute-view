//------------------------------------------------------------------------------
#include "viewmodel.h"
#include "utils/httpgetter.h"
#include "jobmodel.h"
#include "preferences.h"
#include "application.h"

#include <QDebug>
#include <QDomDocument>
//------------------------------------------------------------------------------
ViewModel::ViewModel(const QString &name, const QString &url, QObject *parent):QObject(parent),
	m_name(name),
	m_url(url),
	m_jobsListLoaded(false)
{}
//------------------------------------------------------------------------------
ViewModel::~ViewModel(){
	clearJobs();
}
//------------------------------------------------------------------------------
void ViewModel::setName(const QString &name){
	m_name = name;
}
//------------------------------------------------------------------------------
void ViewModel::setUrl(const QString &url){
	m_url = url;
}
//------------------------------------------------------------------------------
void ViewModel::loadJobs(){
	if(m_url.isEmpty())
		return;

	m_jobsListLoaded = false;

	HttpGetter::instance().get(m_url + "/api/xml", this, &ViewModel::http_finished);
}
//------------------------------------------------------------------------------
const QString & ViewModel::getName() const{
	return m_name;
}
//------------------------------------------------------------------------------
const QString & ViewModel::getUrl() const{
	return m_url;
}
//------------------------------------------------------------------------------
bool ViewModel::isJobsListLoaded() const{
	return m_jobsListLoaded;
}
//------------------------------------------------------------------------------
const ViewModel::JobsList & ViewModel::getJobs() const{
	return m_jobs;
}
//------------------------------------------------------------------------------
void ViewModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"ViewModel["<<m_name<<"]::http_finished("<<errCode<<") : "<<error;
		return;
	}

	QDomDocument doc;
	bool parsed = doc.setContent(content);
	if(!parsed){
		qWarning()<<"ViewModel["<<m_name<<"]::http_finished - Error parsing XML !";
		return;
	}

	parseJobs(doc);
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void ViewModel::parseJobs(const QDomDocument &doc){
	clearJobs();

	if(App.verbose())
		qDebug()<<"ViewModel["<<m_name<<"]::parseJobs()";

	QDomNodeList nodesList = doc.elementsByTagName("job");
	int numJobs = nodesList.size();
	m_jobs.reserve(numJobs);
	for(int i=0 ; i < numJobs ; ++i){
		QDomElement jobElm = nodesList.at(i).toElement();
		if(jobElm.isNull())
			continue;

		QDomElement nameElm = jobElm.firstChildElement("name");
		QDomElement urlElm = jobElm.firstChildElement("url");
		if(nameElm.isNull() || urlElm.isNull())
			continue;

		QString name = nameElm.text();
		QString url = urlElm.text();
		if(name.isEmpty() || url.isEmpty())
			continue;

		JobModel * job = new JobModel(name, url, this);
		QObject::connect(job, &JobModel::loaded, this, &ViewModel::job_loaded);
		m_jobs.push_back(job);
		job->load();
	}
}
//------------------------------------------------------------------------------
void ViewModel::clearJobs(){
	for(JobModel *job : m_jobs)
		job->deleteLater();

	m_jobs.clear();
}
//------------------------------------------------------------------------------
void ViewModel::job_loaded(){
	bool jobsLoaded = true;

	for(JobModel *job : m_jobs){
		if(!job->isLoaded()){
			jobsLoaded = false;
			break;
		}
	}

	if(jobsLoaded){
		m_jobsListLoaded = true;
		emit jobsListLoaded();
	}
}
//------------------------------------------------------------------------------
