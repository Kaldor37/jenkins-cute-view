//------------------------------------------------------------------------------
#include "jenkinscontroller.h"
#include "models/jenkinsxmlapimodel.h"
#include "views/mainwindow.h"
#include "preferences.h"
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
	connect(m_updateTimer, SIGNAL(timeout()), SLOT(timerTimeout()));
	connect(&Prefs, SIGNAL(sigAPIUpdateIntervalChanged(uint)), SLOT(prefs_APIUpdateIntervalChanged(uint)));

	m_XMLAPIModel = new JenkinsXMLAPIModel(this);
	m_XMLAPIModel->setJenkinsUrl(Prefs.getJenkinsUrl());
	m_XMLAPIModel->setSelectedView(Prefs.getSelectedView());
	QObject::connect(&Prefs, SIGNAL(sigJenkinsUrlChanged(QString)), m_XMLAPIModel, SLOT(setJenkinsUrl(QString)));
	QObject::connect(&Prefs, SIGNAL(sigSelectedViewChanged(QString)), m_XMLAPIModel, SLOT(setSelectedView(QString)));
	connect(m_XMLAPIModel, SIGNAL(selectedViewLoaded()), SLOT(selectedViewDataUpdated()));
}
//------------------------------------------------------------------------------
JenkinsController::~JenkinsController(){}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JenkinsController::control(MainWindow *wnd){
	// To UI
	QObject::connect(m_XMLAPIModel, SIGNAL(viewsNamesUpdated(QStringList,QString)), wnd, SIGNAL(viewsNamesUpdated(QStringList,QString)));

	// To graphics view
	QObject::connect(this, SIGNAL(jobs_updated(QList<JobDisplayData>)), wnd->getGraphicsView(), SLOT(updateJobs(QList<JobDisplayData>)));
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
	qDebug()<<"JenkinsController::selectedViewDataUpdated - Jobs to display : "<<jobs.size();

	foreach(const JobModel *job, jobs){
		JobDisplayData jobData;
		jobData.setName(job->getName());
		jobsList.push_back(jobData);
	}

	emit jobs_updated(jobsList);
}
//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------
void JenkinsController::timerTimeout(){
	m_XMLAPIModel->loadViews();
}
//------------------------------------------------------------------------------
