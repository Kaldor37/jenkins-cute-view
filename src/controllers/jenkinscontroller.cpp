//------------------------------------------------------------------------------
#include "jenkinscontroller.h"
#include "models/jenkinsxmlapimodel.h"
#include "models/buildmodel.h"
#include "models/nodemodel.h"
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
JenkinsController::JenkinsController(QObject *parent):QObject(parent){
	m_updateTimer = new QTimer(this);

	m_XMLAPIModel = new JenkinsXMLAPIModel(this);
	QObject::connect(&Prefs, &Preferences::sigJenkinsUrlChanged, m_XMLAPIModel, &JenkinsXMLAPIModel::setJenkinsUrl);
	QObject::connect(&Prefs, &Preferences::sigSelectedViewChanged, m_XMLAPIModel, &JenkinsXMLAPIModel::setSelectedView);
	QObject::connect(m_XMLAPIModel, &JenkinsXMLAPIModel::selectedViewLoaded, this, &JenkinsController::selectedViewDataUpdated);
	QObject::connect(m_XMLAPIModel, &JenkinsXMLAPIModel::nodesListLoaded, this, &JenkinsController::nodesListUpdated);

	QObject::connect(m_updateTimer, &QTimer::timeout, m_XMLAPIModel, &JenkinsXMLAPIModel::loadViews);
	QObject::connect(m_updateTimer, &QTimer::timeout, m_XMLAPIModel, &JenkinsXMLAPIModel::loadNodes);
	QObject::connect(m_updateTimer, &QTimer::timeout, m_XMLAPIModel, &JenkinsXMLAPIModel::loadJobsQueue);
}
//------------------------------------------------------------------------------
JenkinsController::~JenkinsController(){}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JenkinsController::control(MainWindow *wnd){
	qRegisterMetaType< QVector<QString> >("QVector<QString>");
	qRegisterMetaType< QVector<QColor> >("QVector<QColor>");
	qRegisterMetaType< QList<JobDisplayData> >("QList<JobDisplayData>");
	qRegisterMetaType<QStringList>("QStringList");
	qRegisterMetaType<QVector<jenkins::NodeStatus> >("QVector<jenkins::NodeStatus>");

	// To UI
	QObject::connect(m_XMLAPIModel, &JenkinsXMLAPIModel::viewsNamesUpdated, wnd, &MainWindow::viewsNamesUpdated);

	// To graphics view
	// TODO: change these to Qt5 connections
	QObject::connect(this, SIGNAL(jobs_updated(QList<JobDisplayData>)), wnd->getGraphicsView(), SLOT(updateJobs(QList<JobDisplayData>)), Qt::QueuedConnection);
	QObject::connect(this, SIGNAL(nodes_updated(QVector<QString>,QVector<jenkins::NodeStatus>)), wnd->getGraphicsView(), SLOT(updateNodes(QVector<QString>,QVector<jenkins::NodeStatus>)), Qt::QueuedConnection);

	void (JenkinsGraphicsView::*slotMessage)(const QString &) = &JenkinsGraphicsView::displayMessage;
	QObject::connect(m_XMLAPIModel, &JenkinsXMLAPIModel::message, wnd->getGraphicsView(), slotMessage);
	QObject::connect(m_XMLAPIModel, &JenkinsXMLAPIModel::warning, wnd->getGraphicsView(), &JenkinsGraphicsView::displayWarning);
	QObject::connect(m_XMLAPIModel, &JenkinsXMLAPIModel::error, wnd->getGraphicsView(), &JenkinsGraphicsView::displayError);
}
//------------------------------------------------------------------------------
void JenkinsController::prefs_APIUpdateIntervalChanged(uint value){
	if(value > 0)
		m_updateTimer->setInterval(value*1000);
}
//------------------------------------------------------------------------------
void JenkinsController::start(){

	m_XMLAPIModel->setJenkinsUrl(Prefs.getJenkinsUrl());
	m_XMLAPIModel->setSelectedView(Prefs.getSelectedView());

	m_updateTimer->setInterval(Prefs.getAPIUpdateInterval()*1000);
	QObject::connect(&Prefs, &Preferences::sigAPIUpdateIntervalChanged, this, &JenkinsController::prefs_APIUpdateIntervalChanged);

	m_updateTimer->start();
}
//------------------------------------------------------------------------------
void JenkinsController::selectedViewDataUpdated(){
	// Selected view
	const ViewModel *selectedView = m_XMLAPIModel->selectedView();
	Q_ASSERT(selectedView);

	QList<JobDisplayData> jobsList;
	const QVector<QString> &jobsQueue = m_XMLAPIModel->jobsQueue();
	uint queueSize = jobsQueue.size();

	// View's jobs
	const ViewModel::JobsList &jobs = selectedView->getJobs();

	if(App.verbose())
		qDebug()<<"JenkinsController::selectedViewDataUpdated - Jobs to display : "<<jobs.size();

	for(const JobModel *job : jobs){
		const BuildModel *jobLastBuild = job->getLastBuild();
		const BuildModel *jobLastCompBuild = job->getLastCompletedBuild();

		QString displayName = job->getDisplayName();
		QString name = job->getName();

		JobDisplayData jobData;
		jobData.setName(displayName.isEmpty()?name:displayName);
		jobData.setStability(job->getHealthScore());

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
				jobData.setStatus(JobDisplayData::StatusLastBuildSuccessful);
			// Failure
			else if(jobLastCompBuild->getResult() == "FAILURE")
				jobData.setStatus(JobDisplayData::StatusLastBuildFailed);
			// Aborted
			else if(jobLastCompBuild->getResult() == "ABORTED")
				jobData.setStatus(JobDisplayData::StatusInactiveOrAborted);
			// Success but unstable
			else
				jobData.setStatus(JobDisplayData::StatusLastBuildSuccessfulButUnstable);
		}
		// Job never built yet
		else
			jobData.setStatus(JobDisplayData::StatusNeverBuilt);

		if(!job->getBuildable()){
			jobData.setStatus((jobLastCompBuild)?JobDisplayData::StatusInactiveOrAborted:JobDisplayData::StatusInactiveOrAborted);
		}

		// Position in queue
		uint queuePos = 0;
		for(uint i = 0 ; i < queueSize ; ++i){
			if(job->getName() == jobsQueue[i]){
				queuePos = i+1;
				break;
			}
		}
		jobData.setPositionInQueue(queuePos);

		jobsList.push_back(jobData);
	}

	emit jobs_updated(jobsList);
}
//------------------------------------------------------------------------------
void JenkinsController::nodesListUpdated(){
	const JenkinsXMLAPIModel::NodesList &nodesList = m_XMLAPIModel->nodes();

	QVector<QString> nodesNames;
	QVector<jenkins::NodeStatus> nodesStatuses;

	for(const NodeModel * node : nodesList){
		nodesNames.push_back(node->getDisplayName());

		jenkins::NodeStatus status;
		if(node->getTemporarilyOffline())
			status = jenkins::TemporarilyOffline;
		else if(node->getOffline())
			status = jenkins::Offline;
		else if(!node->getIdle())
			status = jenkins::Working;
		else
			status = jenkins::Idle;

		nodesStatuses.push_back(status);
	}

	emit nodes_updated(nodesNames, nodesStatuses);
}
//------------------------------------------------------------------------------
