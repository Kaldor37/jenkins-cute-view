//------------------------------------------------------------------------------
#include "jenkinsgraphicsview.h"
#include <QDebug>
#include <QResizeEvent>
//------------------------------------------------------------------------------
// JenkinsGraphicsScene
//------------------------------------------------------------------------------
JenkinsGraphicsView::JenkinsGraphicsView(QWidget *parent) : QGraphicsView(parent){
	m_scene = new JenkinsGraphicsScene(this);
	m_scene->setSceneRect(0,0,10,10);
	setScene(m_scene);

	m_scene->addText("TODO");
}
//------------------------------------------------------------------------------
JenkinsGraphicsView::~JenkinsGraphicsView(){
	qDebug()<<"JenkinsGraphicsView::~JenkinsGraphicsView";
}
//------------------------------------------------------------------------------
void JenkinsGraphicsView::resizeEvent(QResizeEvent *event){
	const QSize &size = event->size();
	m_scene->setSceneRect(0, 0, size.width(), size.height());
	QGraphicsView::resizeEvent(event);
}
//------------------------------------------------------------------------------
// JenkinsGraphicsScene
//------------------------------------------------------------------------------
JenkinsGraphicsScene::JenkinsGraphicsScene(QWidget *parent):QGraphicsScene(parent){
}
//------------------------------------------------------------------------------
JenkinsGraphicsScene::~JenkinsGraphicsScene(){
	qDebug()<<"JenkinsGraphicsScene::~JenkinsGraphicsScene";
}
//------------------------------------------------------------------------------
