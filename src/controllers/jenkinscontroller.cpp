//------------------------------------------------------------------------------
#include "jenkinscontroller.h"
#include "models/jenkinsapimodel.h"
#include "models/buildmodel.h"
#include "models/nodeslistmodel.h"
#include "models/viewmodel.h"
#include "preferences.h"
#include "views/mainwindow.h"
#include "views/jenkinsgraphicsview.h"
#include "application.h"

#include <QDebug>
#include <QTimer>
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
JenkinsController::JenkinsController(QObject *parent):QObject(parent),
	m_updateTimer(new QTimer(this)),
	m_APIModel(new JenkinsAPIModel(this)),
	m_nodesModel(new NodesListModel(this)){

	QObject::connect(&Prefs, &Preferences::sigJenkinsUrlChanged, m_APIModel, &JenkinsAPIModel::setJenkinsUrl);
	QObject::connect(&Prefs, &Preferences::sigSelectedViewChanged, m_APIModel, &JenkinsAPIModel::setSelectedView);
	QObject::connect(m_APIModel, &JenkinsAPIModel::selectedViewLoaded, this, &JenkinsController::selectedViewDataUpdated);

	QObject::connect(m_updateTimer, &QTimer::timeout, m_APIModel, &JenkinsAPIModel::loadViews);
	QObject::connect(m_updateTimer, &QTimer::timeout, m_APIModel, &JenkinsAPIModel::loadJobsQueue);

	QObject::connect(m_updateTimer, &QTimer::timeout, m_nodesModel, &NodesListModel::update);
	QObject::connect(m_nodesModel, &NodesListModel::updated, this, &JenkinsController::updateNodesList);
}
//------------------------------------------------------------------------------
JenkinsController::~JenkinsController(){}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JenkinsController::control(MainWindow *wnd){
	// To UI
	QObject::connect(m_APIModel, &JenkinsAPIModel::viewsNamesUpdated, wnd, &MainWindow::viewsNamesUpdated);

	// To graphics view
	QObject::connect(this, &JenkinsController::jobs_updated, wnd->getGraphicsView(), &JenkinsGraphicsView::updateJobs);
	QObject::connect(this, &JenkinsController::nodes_updated, wnd->getGraphicsView(), &JenkinsGraphicsView::updateNodes);

	void (JenkinsGraphicsView::*slotMessage)(const QString &) = &JenkinsGraphicsView::displayMessage;
	QObject::connect(m_APIModel, &JenkinsAPIModel::message, wnd->getGraphicsView(), slotMessage);
	QObject::connect(m_APIModel, &JenkinsAPIModel::warning, wnd->getGraphicsView(), &JenkinsGraphicsView::displayWarning);
	QObject::connect(m_APIModel, &JenkinsAPIModel::error, wnd->getGraphicsView(), &JenkinsGraphicsView::displayError);
}
//------------------------------------------------------------------------------
void JenkinsController::prefs_APIUpdateIntervalChanged(uint value){
	if(value > 0)
		m_updateTimer->setInterval(value*1000);
}
//------------------------------------------------------------------------------
void JenkinsController::start(){

	m_APIModel->setJenkinsUrl(Prefs.getJenkinsUrl());
	m_APIModel->setSelectedView(Prefs.getSelectedView());

	m_nodesModel->update();

	m_updateTimer->setInterval(Prefs.getAPIUpdateInterval()*1000);
	QObject::connect(&Prefs, &Preferences::sigAPIUpdateIntervalChanged, this, &JenkinsController::prefs_APIUpdateIntervalChanged);

	m_updateTimer->start();
}
//------------------------------------------------------------------------------
void JenkinsController::selectedViewDataUpdated(){
	// Selected view
	const ViewModel *selectedView = m_APIModel->selectedView();
	Q_ASSERT(selectedView);

	QList<JobDisplayData> jobsList;
	const QVector<QString> &jobsQueue = m_APIModel->jobsQueue();
	uint queueSize = static_cast<uint>(jobsQueue.size());

	// View's jobs
	const ViewModel::JobsList &jobs = selectedView->getJobs();

	if(App.verbose())
		qDebug()<<"JenkinsController::selectedViewDataUpdated - Jobs to display : "<<jobs.size();

	for(const JobModel *job : jobs){
		JobDisplayData::JobType jobType = JobDisplayData::JobType::Unknown;
		if(job->isFreeStyle())
			jobType = JobDisplayData::JobType::Freestyle;
		else if(job->isMultiBranchPipeline())
			jobType = JobDisplayData::JobType::MultibranchPipeline;
		else{
			if(App.verbose())
				qDebug()<<"JenkinsController::selectedViewDataUpdated - Job "<<job->getName()<<" is of unmanged class type "<<job->getJenkinsClass();

			continue;
		}

		QString displayName = job->getDisplayName();
		QString name = job->getName();

		JobDisplayData jobData;
		jobData.setName(displayName.isEmpty()?name:displayName);
		jobData.setStability(static_cast<uint>(job->getHealthScore()));
		jobData.setType(jobType);

		if(job->isFreeStyle()){
			const BuildModel *jobLastBuild = job->getLastBuild();
			const BuildModel *jobLastCompBuild = job->getLastCompletedBuild();

			// Last build infos
			if(jobLastBuild){
				jobData.setLastBuildNum(jobLastBuild->getNumber());
				jobData.setRunning(jobLastBuild->getBuilding());
				if(jobLastBuild->getBuilding()){
					jobData.setStartTime(jobLastBuild->getTimestamp());
					jobData.setEstimatedDuration(jobLastBuild->getEstimatedDuration());
				}
				jobData.setLastBuildDesc(jobLastBuild->getDescription());
			}

			// Last completed build infos
			if(jobLastCompBuild){
				// Success
				if(jobLastCompBuild->getResult() == "SUCCESS")
					jobData.setStatus(JobDisplayData::JobStatus::LastBuildSuccessful);
				// Failure
				else if(jobLastCompBuild->getResult() == "FAILURE")
					jobData.setStatus(JobDisplayData::JobStatus::LastBuildFailed);
				// Aborted
				else if(jobLastCompBuild->getResult() == "ABORTED")
					jobData.setStatus(JobDisplayData::JobStatus::InactiveOrAborted);
				// Success but unstable
				else
					jobData.setStatus(JobDisplayData::JobStatus::LastBuildSuccessfulButUnstable);
			}
			// Job never built yet
			else
				jobData.setStatus(JobDisplayData::JobStatus::NeverBuilt);

			if(!job->getBuildable()){
				jobData.setStatus((jobLastCompBuild)?JobDisplayData::JobStatus::InactiveOrAborted:JobDisplayData::JobStatus::InactiveOrAborted);
			}

			// Position in queue
			uint queuePos = 0;
			for(uint i = 0 ; i < queueSize ; ++i){
				if(job->getName() == jobsQueue[static_cast<int>(i)]){
					queuePos = i+1;
					break;
				}
			}
			jobData.setPositionInQueue(queuePos);
		}
		else if(job->isMultiBranchPipeline()){
			uint succeededJobs = 0;
			uint failedJobs = 0;
			uint totalJobs = 0;
			jobData.setRunning(false);
			for(const JobModel *subJob : job->getJobs()){
				++totalJobs;
				const auto subJobLastCompBuild = subJob->getLastCompletedBuild();
				const auto subJobLastBuild = subJob->getLastBuild();
				if(subJobLastCompBuild){
					succeededJobs += (subJobLastCompBuild->getResult() == "SUCCESS")?1:0;
					failedJobs += (subJobLastCompBuild->getResult() == "FAILURE")?1:0;
				}
				if(subJobLastBuild && subJobLastBuild->getBuilding() && !jobData.isRunning()){
					jobData.setRunning(true);
					jobData.setStartTime(subJobLastBuild->getTimestamp());
					jobData.setEstimatedDuration(subJobLastBuild->getEstimatedDuration());
				}
			}
			jobData.setSucceededJobs(succeededJobs);
			jobData.setFailedJobs(failedJobs);
			jobData.setTotalJobs(totalJobs);
			if(failedJobs == 0 && succeededJobs > 0)
				jobData.setStatus(JobDisplayData::JobStatus::LastBuildSuccessful);
			else if(failedJobs > 0)
				jobData.setStatus(JobDisplayData::JobStatus::LastBuildFailed);
			else
				jobData.setStatus(JobDisplayData::JobStatus::NeverBuilt);
		}

		jobsList.push_back(jobData);
	}

	emit jobs_updated(jobsList);
}
//------------------------------------------------------------------------------
void JenkinsController::updateNodesList(){
	const NodesListModel::NodesList &nodesList = m_nodesModel->nodes();

	QVector<JenkinsGraphicsView::NodeData> nodes;
	nodes.reserve(nodesList.size());

	for(const NodesListModel::Node & node : nodesList){
		JenkinsGraphicsView::NodeData nd;
		nd.name = node.displayName;
		nd.responseTime = node.reponseTime;
		if(node.temporarilyOffline)
			nd.status = JenkinsGraphicsView::NodeStatus::TemporarilyOffline;
		else if(node.offline)
			nd.status = JenkinsGraphicsView::NodeStatus::Offline;
		else if(!node.idle)
			nd.status = JenkinsGraphicsView::NodeStatus::Working;
		else
			nd.status = JenkinsGraphicsView::NodeStatus::Idle;

		nodes.push_back(nd);
	}

	emit nodes_updated(nodes);
}
//------------------------------------------------------------------------------
