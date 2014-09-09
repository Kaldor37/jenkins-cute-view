//------------------------------------------------------------------------------
#include "jenkinsgraphicsview.h"
#include "graphicsitems/jobgraphicsitem.h"
#include "graphicsitems/nodegraphicsitem.h"
#include "graphicsitems/autoresizingtextitem.h"
#include "preferences.h"
#include "application.h"

#include <QDebug>
#include <QResizeEvent>
#include <QTimer>
#include <QMenu>
#include <qmath.h>
//------------------------------------------------------------------------------
// JenkinsGraphicsScene
//------------------------------------------------------------------------------
JenkinsGraphicsView::JenkinsGraphicsView(QWidget *parent) : QGraphicsView(parent),
	m_contextMenu(0),
	m_fullscreenAction(0),
	m_preferencesAction(0),
	m_quitAction(0)
{
	m_scene = new JenkinsGraphicsScene(this);
	setScene(m_scene);

	m_progressUpdateTimer = new QTimer(this);
	connect(m_progressUpdateTimer, SIGNAL(timeout()), SLOT(progressTimer_timeout()));
	m_progressUpdateTimer->start(jobsProgressUpdateTimer);

	m_messageItem = new MessageGraphicsItem(); // Destroyed by the scene
	m_messageItem->setVisible(false);
	m_scene->addItem(m_messageItem);

	m_columns = Prefs.getColumns();
	connect(&Prefs, SIGNAL(sigColumnsChanged(uint)), SLOT(setColumns(uint)));

	m_showNodes = Prefs.getShowNodes();
	connect(&Prefs, SIGNAL(sigShowNodesChanged(bool)), SLOT(setShowNodes(bool)));

	m_jobsMargin = Prefs.getJobsMargin();
	connect(&Prefs, SIGNAL(sigJobsMarginChanged(uint)), SLOT(setJobsMargin(uint)));

	initContextMenu();
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
void JenkinsGraphicsView::contextMenuEvent(QContextMenuEvent *event){
	Q_ASSERT(m_contextMenu);

	emit contextMenuShown();

	m_contextMenu->exec(event->globalPos());

	emit contextMenuHidden();

	QGraphicsView::contextMenuEvent(event);
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::updateJobs(const QList<JobDisplayData> &jobs){
	QVector<QString> jobsList;
	QVector<QString> jobsDeleteList;

	if(App.verbose())
		qDebug()<<"JenkinsGraphicsView::updateJobs("<<jobs.size()<<")";

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
		}
		// Job not found, add
		else{
			JobGraphicsItem *newJob = new JobGraphicsItem(this); // Deleted with scene
			newJob->setName(name);
			newJob->update(job);
			m_jobItems[name] = newJob;
			m_scene->addItem(newJob);
		}
	}

	// Builing job delete list
	const JobsItems::Iterator &end = m_jobItems.end();
	for(JobsItems::Iterator it = m_jobItems.begin() ; it != end; ++it){
		const QString &name = it.key();
		if(!jobsList.contains(name))
			jobsDeleteList.push_back(name);
	}

	foreach(const QString &name, jobsDeleteList){
		Q_ASSERT(m_jobItems.contains(name));
		JobGraphicsItem *jobItem = m_jobItems.take(name);
		m_scene->removeItem(jobItem);
		jobItem->deleteLater();
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
void JenkinsGraphicsView::updateNodes(const QVector<QString> &nodeNames, const QVector<jenkins::NodeStatus> &nodeStatuses){
	Q_ASSERT(nodeNames.size() == nodeStatuses.size());
	int nbNodes = nodeNames.size();

	for(int i=0 ; i < nbNodes ; ++i){
		const QString &name = nodeNames.at(i);

		const jenkins::NodeStatus &nodeStatus = nodeStatuses.at(i);
		QColor color;
		switch(nodeStatus){
			case jenkins::Idle:						color = property("NodeIdle").value<QColor>(); break;
			case jenkins::Working:					color = property("NodeIdle").value<QColor>().lighter(); break;
			case jenkins::Offline:					color = property("NodeOffline").value<QColor>(); break;
			case jenkins::TemporarilyOffline:	color = property("NodeTemporarilyOffline").value<QColor>(); break;
		}

		// Find node
		NodeGraphicsItem *foundNode = NULL;
		NodesItems::Iterator found = m_nodeItems.find(name);
		if(found != m_nodeItems.end()){
			foundNode = found.value();
			Q_ASSERT(foundNode);
		}

		// Update node
		if(foundNode){
			foundNode->setColor(color);
		}
		// Create node
		else{
			NodeGraphicsItem *nodeItem = new NodeGraphicsItem();
			nodeItem->setName(name);
			nodeItem->setColor(color);
			m_nodeItems[name] = nodeItem;
			m_scene->addItem(nodeItem);
		}
	}

	// Delete old nodes
	QList<QString> nodesList = m_nodeItems.keys();
	foreach(const QString &nodeName, nodesList){
		if(!nodeNames.contains(nodeName)){
			NodeGraphicsItem *node = m_nodeItems.take(nodeName);
			m_scene->removeItem(node);
			node->deleteLater();
		}
	}

	updateDisplay();
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
void JenkinsGraphicsView::fullscreenModeChanged(bool enabled){
	if(m_fullscreenAction)
		m_fullscreenAction->setChecked(enabled);
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::setColumns(uint value){
	Q_ASSERT(value > 0);
	if(value == 0)
		return;

	m_columns = value;
	updateDisplay();
	update();
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::setShowNodes(bool value){
	m_showNodes = value;
	updateDisplay();
	update();
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::setJobsMargin(uint value){
	m_jobsMargin = value;
	updateDisplay();
	update();
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::updateDisplay(){
	QSizeF size = m_scene->sceneRect().size();
	qreal width = size.width();
	qreal height = size.height();

	const JobsItems::Iterator end = m_jobItems.end();
	const NodesItems::Iterator nodesEnd = m_nodeItems.end();

	// Display message instead of jobs
	if(m_messageItem->isVisible()){
		m_messageItem->setRect(QRectF(width*0.05, height*0.4, width*0.9, height*0.2));

		// Hide jobs
		for(JobsItems::Iterator it=m_jobItems.begin() ; it != end ; ++it){
			JobGraphicsItem *job = it.value();
			Q_ASSERT(job);
			job->setVisible(false);
		}
		// Hide nodes
		for(NodesItems::Iterator it=m_nodeItems.begin() ; it != nodesEnd ; ++it){
			NodeGraphicsItem *node = it.value();
			Q_ASSERT(node);
			node->setVisible(false);
		}

		return;
	}

	Q_ASSERT(m_columns > 0);
	if(m_columns == 0)
		return;
	if(m_jobItems.size() <= 0)
		return;

	// Number of nodes to display
	qreal numNodes = (m_showNodes)?m_nodeItems.size():0;
	// Number of jobs
	qreal numJobs = m_jobItems.size();
	// Number of columns
	qreal numColumns = (m_columns > numJobs)?numJobs:m_columns;
	// Jobs per column
	qreal jobsPerCol = qCeil(numJobs/numColumns);
	// Number of rows
	qreal numRows = jobsPerCol+((numNodes > 0)?1:0);

	// Job width
	qreal jobWidth = (width-((numColumns+1)*m_jobsMargin))/numColumns;
	// Job height
	qreal jobHeight = (height-((numRows+1)*m_jobsMargin))/numRows;
	// Node width
	qreal nodeWidth = (numNodes > 0)?((width-((numNodes+1)*m_jobsMargin))/numNodes):0;

	int i = 0;

	if(m_showNodes){
		for(NodesItems::Iterator it=m_nodeItems.begin() ; it != nodesEnd ; ++it){
			NodeGraphicsItem *node = it.value();
			Q_ASSERT(node);
			node->setRect(QRectF(m_jobsMargin + ((nodeWidth+m_jobsMargin)*i), m_jobsMargin, nodeWidth, jobHeight));

			if(!node->isVisible())
				node->setVisible(true);

			++i;
		}
	}

	int col = 0, row = ((numNodes > 0)?1:0);
	for(JobsItems::Iterator it=m_jobItems.begin() ; it != end ; ++it){

		JobGraphicsItem *job = it.value();
		Q_ASSERT(job);
		job->setRect(QRectF(m_jobsMargin + ((jobWidth+m_jobsMargin)*col), m_jobsMargin + ((jobHeight+m_jobsMargin)*row), jobWidth, jobHeight));

		if(!job->isVisible())
			job->setVisible(true);

		++row;
		if(row > jobsPerCol){
			col++;
			row = ((numNodes > 0)?1:0);
		}
	}
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::initContextMenu(){
	Q_ASSERT(!m_contextMenu);

	m_contextMenu = new QMenu(qobject_cast<QWidget*>(parent()));

	m_fullscreenAction = m_contextMenu->addAction(tr("Fullscreen"));
	m_fullscreenAction->setCheckable(true);
	connect(m_fullscreenAction, SIGNAL(triggered()), SIGNAL(fullScreenTriggered()));

	m_preferencesAction = m_contextMenu->addAction(tr("Preferences"));
	connect(m_preferencesAction, SIGNAL(triggered()), SIGNAL(preferencesTriggered()));

	m_contextMenu->addSeparator();

	m_quitAction = m_contextMenu->addAction(tr("Quit"));
	connect(m_quitAction, SIGNAL(triggered()), SIGNAL(quitTriggered()));
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::progressTimer_timeout(){
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

	if(repaint){
		this->update();
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
