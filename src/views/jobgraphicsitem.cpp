//------------------------------------------------------------------------------
#include "views/jobgraphicsitem.h"

#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QPainter>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JobGraphicsItem::JobGraphicsItem(QGraphicsItem *parent/* = 0*/):QGraphicsObject(parent){

}
//------------------------------------------------------------------------------
JobGraphicsItem::~JobGraphicsItem(){

}
//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
QRectF JobGraphicsItem::boundingRect() const{
	return QRectF(0, 0, m_size.width(), m_size.height());
}
//------------------------------------------------------------------------------
void JobGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option);
	Q_UNUSED(widget);

	const QRectF rect = boundingRect();

	painter->save();

	// Background rectangle
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGreen);
	painter->drawRoundedRect(rect, 10, 10);

	// Job name
	painter->setPen(Qt::white);
	painter->setFont(QFont("Arial", 14));
	painter->drawText(rect,Qt::AlignCenter,m_name);

	painter->restore();
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JobGraphicsItem::setPos(const QPointF & pos){
	QGraphicsItem::setPos(pos);
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setPos(qreal x, qreal y){
	QGraphicsItem::setPos(x, y);
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setSize(qreal width, qreal height){
	m_size = QSizeF(width, height);
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setSize(const QSizeF & size){
	m_size = size;
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	setSize(rect.width(), rect.height());
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setName(const QString &name){
	m_name = name;
}
//------------------------------------------------------------------------------
const QString & JobGraphicsItem::getName() const{
	return m_name;
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void JobGraphicsItem::rectChanged(){

}
//------------------------------------------------------------------------------
