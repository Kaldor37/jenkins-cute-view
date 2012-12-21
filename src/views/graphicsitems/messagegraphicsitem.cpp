//------------------------------------------------------------------------------
#include "messagegraphicsitem.h"
#include "autoresizingtextitem.h"

#include <QPainter>

//#define DEBUG_RECT 1
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
MessageGraphicsItem::MessageGraphicsItem(QGraphicsItem *parent):QGraphicsObject(parent){
	m_messageItem = new AutoResizingTextItem(this); // Deleted with parent
	m_messageItem->setPen(QPen(Qt::white));
	m_messageItem->setFont(QFont("Arial"));
}
//------------------------------------------------------------------------------
MessageGraphicsItem::~MessageGraphicsItem(){}
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
QRectF MessageGraphicsItem::boundingRect() const{
	return QRectF(0,0,m_size.width(),m_size.height());
}
//------------------------------------------------------------------------------
void MessageGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option);
	Q_UNUSED(widget);

	painter->save();
	painter->setBrush(m_backgroundColor);
	painter->setPen(QPen(Qt::white));
	painter->drawRoundedRect(boundingRect(), 10, 10);
	painter->restore();
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void MessageGraphicsItem::setSize(qreal w, qreal h){
	m_size = QSizeF(w,h);
	updateMessage();
}
//------------------------------------------------------------------------------
void MessageGraphicsItem::setSize(const QSizeF & size){
	m_size = size;
	updateMessage();
}
//------------------------------------------------------------------------------
void MessageGraphicsItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	setSize(rect.width(), rect.height());
}
//------------------------------------------------------------------------------
void MessageGraphicsItem::setMessage(eMessageType type, const QString &msg){
	switch(type){
		case NoType:	m_backgroundColor = QColor(); break;
		case Normal:	m_backgroundColor = Qt::darkGray; break;
		case Warning:	m_backgroundColor = Qt::darkYellow; break;
		case Error:		m_backgroundColor = Qt::darkRed; break;
	}
	m_messageItem->setText(msg);
	updateMessage();
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void MessageGraphicsItem::updateMessage(){
	QRectF rect(boundingRect());
	m_messageItem->setRect(QRectF(rect.width()*0.1, rect.height()*0.1, rect.width()*0.8, rect.height()*0.8));
	m_messageItem->setVisible(m_backgroundColor.isValid() && !m_messageItem->text().isEmpty());
}
//------------------------------------------------------------------------------
