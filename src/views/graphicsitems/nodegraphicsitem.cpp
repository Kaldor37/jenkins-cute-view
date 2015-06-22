//------------------------------------------------------------------------------
#include "nodegraphicsitem.h"
#include "autoresizingtextitem.h"
#include "preferences.h"

#include <QBrush>
#include <QPen>
#include <QPainter>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
NodeGraphicsItem::NodeGraphicsItem(QGraphicsItem *parent/* = nullptr*/):QGraphicsObject(parent),
	m_nameItem(new AutoResizingTextItem(this)),
	m_pingItem(new AutoResizingTextItem(this)){

	m_nameItem->setFont(QFont(Prefs.getFont(), -1, QFont::Bold));
	m_nameItem->setPen(QPen(Qt::white));
	m_nameItem->setShadowed(true);
	QObject::connect(&Prefs, &Preferences::sigFontChanged, m_nameItem, &AutoResizingTextItem::setFontFamily);

	m_pingItem->setTextFlags(Qt::AlignCenter);
	m_pingItem->setFont(QFont(Prefs.getFont(), -1, QFont::Bold));
	m_pingItem->setPen(QPen(Qt::white));
	QObject::connect(&Prefs, &Preferences::sigFontChanged, m_pingItem, &AutoResizingTextItem::setFontFamily);
}
//------------------------------------------------------------------------------
NodeGraphicsItem::~NodeGraphicsItem(){ }
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
QRectF NodeGraphicsItem::boundingRect() const{
	return QRectF(0, 0, m_size.width(), m_size.height());
}
//------------------------------------------------------------------------------
void NodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option);
	Q_UNUSED(widget);

	const QRectF rect = boundingRect();

	painter->save();

	// Background rectangle
	painter->setPen(Qt::NoPen);
	painter->setBrush(m_color);
	painter->drawRoundedRect(rect, 10, 10);

	painter->restore();
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void NodeGraphicsItem::setSize(qreal width, qreal height){
	m_size = QSizeF(width, height);
	updateLayout();
}
//------------------------------------------------------------------------------
void NodeGraphicsItem::setSize(const QSizeF & size){
	m_size = size;
	updateLayout();
}
//------------------------------------------------------------------------------
void NodeGraphicsItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	setSize(rect.width(), rect.height());
	updateLayout();
}
//------------------------------------------------------------------------------
void NodeGraphicsItem::setName(const QString &name){
	m_name = name;
	Q_ASSERT(m_nameItem);
	m_nameItem->setText(m_name);
}
//------------------------------------------------------------------------------
void NodeGraphicsItem::setPing(uint ping){
	Q_ASSERT(m_pingItem);
	m_pingItem->setVisible(ping > 0);
	m_pingItem->setText(QString("%1 ms").arg(ping));
}
//------------------------------------------------------------------------------
void NodeGraphicsItem::setColor(const QColor &color){
	m_color = color;
}
//------------------------------------------------------------------------------
const QString & NodeGraphicsItem::getName() const{
	return m_name;
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void NodeGraphicsItem::updateLayout(){
	QRectF rect(boundingRect());
	m_nameItem->setRect(QRectF(rect.x()+(rect.width()*0.05), rect.height()*0.2, rect.width()*0.9, rect.height()*0.6));
	m_pingItem->setRect(QRectF(rect.x()+(rect.width()*0.1), rect.height()*0.75, rect.width()*0.9, rect.height()*0.18));
}
//------------------------------------------------------------------------------
