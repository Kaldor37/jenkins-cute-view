//------------------------------------------------------------------------------
#include "jenkinsgraphicsview.h"
#include "jobgraphicsitem.h"

#include <QDebug>
#include <QResizeEvent>
//------------------------------------------------------------------------------
// JenkinsGraphicsScene
//------------------------------------------------------------------------------
JenkinsGraphicsView::JenkinsGraphicsView(QWidget *parent) : QGraphicsView(parent),
	m_jobItems()
{
	m_scene = new JenkinsGraphicsScene(this);
	m_scene->setSceneRect(0,0,10,10);
	setScene(m_scene);
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

	// Add new jobs - Update existing jobs
	foreach(const JobDisplayData &job, jobs){
		const QString &name = job.getName();

		// Searching for the job
		JobGraphicsItem *foundJob = NULL;
		foreach(JobGraphicsItem *jobItem, m_jobItems){
			Q_ASSERT(jobItem);
			if(jobItem && jobItem->getName() == name){
				foundJob = jobItem;
				break;
			}
		}

		// Update Job
		if(foundJob){
			foundJob->setName(name);
			qDebug()<<"Updated job "<<name;
		}
		// Add job
		else{
			JobGraphicsItem *newJob = new JobGraphicsItem(); // Deleted with scene
			newJob->setName(job.getName());
			m_jobItems.push_back(newJob);
			m_scene->addItem(newJob);
			qDebug()<<"Added job "<<name;
		}

		jobsList.push_back(name);
	}

	// Delete jobs
	for(JobsItems::Iterator it=m_jobItems.begin() ; it != m_jobItems.end() ; ++it){
		JobGraphicsItem *jobItem = *it;
		const QString &name = jobItem->getName();
		if(!jobsList.contains(name)){
			m_scene->removeItem(jobItem);
			jobItem->deleteLater();
			qDebug()<<"Removed job "<<name;

			it = m_jobItems.erase(it);
			if(it == m_jobItems.end())
				break;
		}
	}

	updateDisplay();
	repaint();
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::updateDisplay(){
	QSizeF size = m_scene->sceneRect().size();
	qreal width = size.width();
	qreal height = size.height();

	// Resize jobs
	qreal numJobs = m_jobItems.size();
	qreal jobWidth = width;
	qreal jobHeight = height/numJobs;

	for(int i=0 ; i < numJobs ; ++i){
		JobGraphicsItem *job = m_jobItems.at(i);
		Q_ASSERT(job);
		job->setRect(QRectF(jobsMargin, (jobHeight*i)+jobsMargin, jobWidth-(2*jobsMargin), jobHeight-(2*jobsMargin)));
	}
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
