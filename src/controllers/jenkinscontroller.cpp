//------------------------------------------------------------------------------
#include "jenkinscontroller.h"
#include "models/jenkinsxmlapimodel.h"
#include "models/buildmodel.h"
#include "models/nodemodel.h"
#include "models/viewmodel.h"
#include "preferences.h"
#include "views/mainwindow.h"
#include "views/jenkinsgraphicsview.h"

#include <QDebug>
#include <QTimer>
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
JenkinsController::JenkinsController(QObject *parent/*=0*/):
	QObject(parent),
	m_updateTimer(0)
{
	m_updateTimer = new QTimer(this);
	m_updateTimer->setInterval(Prefs.getAPIUpdateInterval()*1000);
	connect(&Prefs, SIGNAL(sigAPIUpdateIntervalChanged(uint)), SLOT(prefs_APIUpdateIntervalChanged(uint)));

	m_XMLAPIModel = new JenkinsXMLAPIModel(this);
	m_XMLAPIModel->setJenkinsUrl(Prefs.getJenkinsUrl());
	m_XMLAPIModel->setSelectedView(Prefs.getSelectedView());
	QObject::connect(&Prefs, SIGNAL(sigJenkinsUrlChanged(QString)), m_XMLAPIModel, SLOT(setJenkinsUrl(QString)));
	QObject::connect(&Prefs, SIGNAL(sigSelectedViewChanged(QString)), m_XMLAPIModel, SLOT(setSelectedView(QString)));
	connect(m_XMLAPIModel, SIGNAL(selectedViewLoaded()), SLOT(selectedViewDataUpdated()));
	connect(m_XMLAPIModel, SIGNAL(nodesListLoaded()), SLOT(nodesListUpdated()));

	QObject::connect(m_updateTimer, SIGNAL(timeout()), m_XMLAPIModel, SLOT(loadViews()));
	QObject::connect(m_updateTimer, SIGNAL(timeout()), m_XMLAPIModel, SLOT(loadNodes()));
	QObject::connect(m_updateTimer, SIGNAL(timeout()), m_XMLAPIModel, SLOT(loadQueue()));
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

	// To UI
	QObject::connect(m_XMLAPIModel, SIGNAL(viewsNamesUpdated(QStringList,QString)), wnd, SIGNAL(viewsNamesUpdated(QStringList,QString)));

	// To graphics view
	QObject::connect(this, SIGNAL(jobs_updated(QList<JobDisplayData>)), wnd->getGraphicsView(), SLOT(updateJobs(QList<JobDisplayData>)), Qt::QueuedConnection);
	QObject::connect(this, SIGNAL(nodes_updated(QVector<QString>,QVector<QColor>)), wnd->getGraphicsView(), SLOT(updateNodes(QVector<QString>,QVector<QColor>)), Qt::QueuedConnection);

	QObject::connect(m_XMLAPIModel, SIGNAL(message(QString)), wnd->getGraphicsView(), SLOT(displayMessage(QString)));
	QObject::connect(m_XMLAPIModel, SIGNAL(warning(QString)), wnd->getGraphicsView(), SLOT(displayWarning(QString)));
	QObject::connect(m_XMLAPIModel, SIGNAL(error(QString)), wnd->getGraphicsView(), SLOT(displayError(QString)));
}
//------------------------------------------------------------------------------
void JenkinsController::prefs_APIUpdateIntervalChanged(uint value){
	if(value > 0)
		m_updateTimer->setInterval(value*1000);
}
//------------------------------------------------------------------------------
void JenkinsController::start(){
	m_updateTimer->start();
}
//------------------------------------------------------------------------------
void JenkinsController::selectedViewDataUpdated(){
	// Selected view
	const ViewModel *selectedView = m_XMLAPIModel->selectedView();
	Q_ASSERT(selectedView);

	QList<JobDisplayData> jobsList;

	// View's jobs
	const ViewModel::JobsList &jobs = selectedView->getJobs();
	//qDebug()<<"JenkinsController::selectedViewDataUpdated - Jobs to display : "<<jobs.size();

	foreach(const JobModel *job, jobs){
		const BuildModel *jobLastBuild = job->getLastBuild();
		const BuildModel *jobLastCompBuild = job->getLastCompletedBuild();

		JobDisplayData jobData;
		jobData.setName(job->getName());
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
				jobData.setStatus(JobDisplayData::StatusInactiveOrNeverBuilt);
			// Success but unstable
			else
				jobData.setStatus(JobDisplayData::StatusLastBuildSuccessfulButUnstable);
		}
		// Job never built yet
		else
			jobData.setStatus(JobDisplayData::StatusInactiveOrNeverBuilt);

		if(!job->getBuildable())
			jobData.setStatus(JobDisplayData::StatusInactiveOrNeverBuilt);

		jobsList.push_back(jobData);
	}

	emit jobs_updated(jobsList);
}
//------------------------------------------------------------------------------
void JenkinsController::nodesListUpdated(){
	const JenkinsXMLAPIModel::NodesList &nodesList = m_XMLAPIModel->nodes();

	QVector<QString> nodesNames;
	QVector<QColor> nodesColors;

	foreach(const NodeModel * node, nodesList){
		nodesNames.push_back(node->getDisplayName());

		QColor nodeColor = Qt::darkGreen;
		if(node->getTemporarilyOffline())
			nodeColor = Qt::darkGray;
		else if(node->getOffline())
			nodeColor = Qt::darkRed;
		else if(!node->getIdle())
			nodeColor = QColor(Qt::darkGreen).lighter();

		nodesColors.push_back(nodeColor);
	}

	emit nodes_updated(nodesNames, nodesColors);
}
//------------------------------------------------------------------------------
