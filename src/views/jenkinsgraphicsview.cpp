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

	// Delete jobs
	for(JobsItems::Iterator it=m_jobItems.begin() ; it != m_jobItems.end() ; ++it){
		JobGraphicsItem *jobItem = *it;
		const QString &name = jobItem->getName();
		if(!jobsList.contains(name)){
			m_scene->removeItem(jobItem);
			jobItem->deleteLater();
			//qDebug()<<"Removed job "<<name;

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
	qreal margin = jobsMargin;

	// Resize jobs
	qreal numJobs = m_jobItems.size();
	qreal jobWidth = width-(2*margin);
	qreal jobHeight = (height-((numJobs+1)*margin))/numJobs;

	int i=0;
	const JobsItems::Iterator end = m_jobItems.end();
	for(JobsItems::Iterator it=m_jobItems.begin() ; it != end ; ++it){
		JobGraphicsItem *job = it.value();
		Q_ASSERT(job);
		job->setRect(QRectF(margin, margin + ((jobHeight+margin)*i), jobWidth, jobHeight));
		++i;
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
