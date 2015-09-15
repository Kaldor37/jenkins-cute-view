//------------------------------------------------------------------------------
#include "viewmodel.h"
#include "utils/httpgetter.h"
#include "jobmodel.h"
#include "preferences.h"
#include "application.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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

	HttpGetter::instance().get(m_url + "/api/json", this, &ViewModel::http_finished);
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
ViewModel::JobsList ViewModel::getJobs() const {
	return m_jobs.values();
}
//------------------------------------------------------------------------------
void ViewModel::http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error){
	if(errCode != QNetworkReply::NoError){
		qWarning()<<"ViewModel["<<m_name<<"]::http_finished("<<errCode<<") : "<<error;
		return;
	}

	QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());
	Q_ASSERT(jsonDoc.isObject());
	const QJsonObject rootObj = jsonDoc.object();

	Q_ASSERT(rootObj.contains("jobs"));
	Q_ASSERT(rootObj["jobs"].isArray());
	const QJsonArray jobs = rootObj["jobs"].toArray();

	QStringList jobNames;
	jobNames.reserve(jobs.size());

	for(const QJsonValue jJob : jobs){
		Q_ASSERT(jJob.isObject());
		const QJsonObject joJob = jJob.toObject();
		const QString name = joJob["name"].toString();
		Q_ASSERT(!name.isEmpty());
		const QString url = joJob["url"].toString();
		Q_ASSERT(!url.isEmpty());

		JobModel * job = nullptr;
		if(m_jobs.contains(name)){
			job = m_jobs[name];
			job->setUrl(url);
		}
		else{
			job = new JobModel(name, url, this);
			QObject::connect(job, &JobModel::loaded, this, &ViewModel::job_loaded);
			m_jobs[name] = job;
		}
		Q_ASSERT(job);
		job->load();
		jobNames.append(name);
	}

	for(JobsMap::iterator it=m_jobs.begin() ; it != m_jobs.end() ; ){
		if(!jobNames.contains(it.key())){
			JobModel * model = it.value();
			model->disconnect(this);
			model->deleteLater();
			it = m_jobs.erase(it);
		}
		else
			++it;
	}
}
//------------------------------------------------------------------------------
// Private functions
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
