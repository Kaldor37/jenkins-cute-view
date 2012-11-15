//------------------------------------------------------------------------------
#include "jenkinsgraphicsview.h"
#include "jobgraphicsitem.h"
#include "autoresizingtextitem.h"

#include <QDebug>
#include <QResizeEvent>
#include <QTimer>
#include <QMutexLocker>
//------------------------------------------------------------------------------
// JenkinsGraphicsScene
//------------------------------------------------------------------------------
JenkinsGraphicsView::JenkinsGraphicsView(QWidget *parent) : QGraphicsView(parent){
	m_scene = new JenkinsGraphicsScene(this);
	setScene(m_scene);

	m_progressUpdateTimer = new QTimer(this);
	connect(m_progressUpdateTimer, SIGNAL(timeout()), SLOT(progressTimer_timeout()));
	m_progressUpdateTimer->start(jobsProgressUpdateTimer);

	m_messageItem = new MessageGraphicsItem();
	m_messageItem->setVisible(false);
	m_scene->addItem(m_messageItem);
}
//------------------------------------------------------------------------------
JenkinsGraphicsView::~JenkinsGraphicsView(){
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::resizeEvent(QResizeEvent *event){
	// Update scene rect
	m_scene->setSceneRect(QRectF(QPointF(0,0), event->size()));

	// Update display
	updateDisplay();

	QGraphicsView::resizeEvent(event);
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::updateJobs(const QList<JobDisplayData> &jobs){
	QVector<QString> jobsList;
	QVector<QString> jobsDeleteList;

	{
		QMutexLocker locker(&m_jobsMutex);
		Q_UNUSED(locker);

		// Add new jobs - Update existing jobs
		foreach(const JobDisplayData &job, jobs){
			const QString &name = job.getName();
			jobsList.push_back(name);

			// Find job
			JobGraphicsItem *foundJob = NULL;
			JobsItems::Iterator found = m_jobItems.find(name);
			if(found != m_jobItems.end()){
				foundJob = found.value();
				Q_ASSERT(foundJob);
			}

			// Job found, update
			if(foundJob){
				foundJob->setName(name);
				foundJob->update(job);
				//qDebug()<<"Updated job "<<name;
			}
			// Job not found, add
			else{
				JobGraphicsItem *newJob = new JobGraphicsItem(); // Deleted with scene
				newJob->setName(name);
				newJob->update(job);
				m_jobItems[name] = newJob;
				m_scene->addItem(newJob);
				//qDebug()<<"Added job "<<name;
			}
		}

		// Builing job delete list
		const JobsItems::Iterator &end = m_jobItems.end();
		for(JobsItems::Iterator it = m_jobItems.begin() ; it != end; ++it){
			const QString &name = it.key();
			if(!jobsList.contains(name))
				jobsDeleteList.push_back(name);
		}

		foreach(QString name, jobsDeleteList){
			Q_ASSERT(m_jobItems.contains(name));
			JobGraphicsItem *jobItem = m_jobItems.take(name);
			m_scene->removeItem(jobItem);
			jobItem->deleteLater();
			//qDebug()<<"Removed job "<<name;
		}
	}

	// Nothing to display
	if(m_jobItems.size() <= 0){
		displayMessage(tr("No jobs to display"));
	}
	// Hide error
	else{
		m_messageItem->setVisible(false);
		updateDisplay();
	}

	update();
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::displayMessage(const QString & msg, MessageGraphicsItem::eMessageType type){
	Q_ASSERT(!msg.isEmpty());
	m_messageItem->setMessage(type, msg);
	m_messageItem->setVisible(true);
	updateDisplay();
	update();
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::displayWarning(const QString & msg){
	displayMessage(msg, MessageGraphicsItem::Warning);
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::displayError(const QString &msg){
	displayMessage(msg, MessageGraphicsItem::Error);
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::updateDisplay(){
	QMutexLocker locker(&m_jobsMutex);
	Q_UNUSED(locker);

	QSizeF size = m_scene->sceneRect().size();
	qreal width = size.width();
	qreal height = size.height();
	qreal margin = jobsMargin;

	const JobsItems::Iterator end = m_jobItems.end();

	// Display message instead of jobs
	if(m_messageItem->isVisible()){
		m_messageItem->setRect(QRectF(width*0.05, height*0.4, width*0.9, height*0.2));

		// Hide jobs
		for(JobsItems::Iterator it=m_jobItems.begin() ; it != end ; ++it){
			JobGraphicsItem *job = it.value();
			Q_ASSERT(job);
			job->setVisible(false);
		}

		return;
	}

	// Resize jobs
	qreal numJobs = m_jobItems.size();
	qreal jobWidth = width-(2*margin);
	qreal jobHeight = (height-((numJobs+1)*margin))/numJobs;

	int i=0;
	for(JobsItems::Iterator it=m_jobItems.begin() ; it != end ; ++it){
		JobGraphicsItem *job = it.value();
		Q_ASSERT(job);
		job->setRect(QRectF(margin, margin + ((jobHeight+margin)*i), jobWidth, jobHeight));

		if(!job->isVisible())
			job->setVisible(true);

		++i;
	}
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::progressTimer_timeout(){
	QMutexLocker locker(&m_jobsMutex);
	Q_UNUSED(locker);

	bool repaint = false;

	// Update running jobs
	const JobsItems::Iterator end = m_jobItems.end();
	for(JobsItems::Iterator it=m_jobItems.begin() ; it != end ; ++it){
		JobGraphicsItem *job = it.value();
		if(job->isRunning()){
			job->updateProgress();
			repaint = true;
		}
	}

	if(repaint)
		this->update();
}
//------------------------------------------------------------------------------
// JenkinsGraphicsScene
//------------------------------------------------------------------------------
JenkinsGraphicsScene::JenkinsGraphicsScene(QWidget *parent):QGraphicsScene(parent){
}
//------------------------------------------------------------------------------
JenkinsGraphicsScene::~JenkinsGraphicsScene(){
}
//------------------------------------------------------------------------------
