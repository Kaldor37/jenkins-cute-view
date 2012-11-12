//------------------------------------------------------------------------------
#include "views/jobgraphicsitem.h"

#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QPainter>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JobGraphicsItem::JobGraphicsItem(QGraphicsItem *parent/* = 0*/):QGraphicsObject(parent),
	m_nameItem(0)
{
	m_nameItem = new AutoResizingTextItem(this);
	m_nameItem->setFont(QFont("Arial")); // TODO - Manage in prefs
	m_nameItem->setPen(QPen(Qt::white));
}
//------------------------------------------------------------------------------
JobGraphicsItem::~JobGraphicsItem(){
	if(m_nameItem)
		m_nameItem->deleteLater();
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
	painter->setBrush(m_rectColor);
	painter->drawRoundedRect(rect, 20, 20);

	painter->restore();
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void JobGraphicsItem::setSize(qreal width, qreal height){
	m_size = QSizeF(width, height);
	rectChanged();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setSize(const QSizeF & size){
	m_size = size;
	rectChanged();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	setSize(rect.width(), rect.height());
	rectChanged();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setName(const QString &name){
	Q_ASSERT(m_nameItem);
	m_nameItem->setText(name);
}
//------------------------------------------------------------------------------
const QString & JobGraphicsItem::getName() const{
	Q_ASSERT(m_nameItem);
	return m_nameItem->text();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::update(const JobDisplayData& data){
	switch(data.getStatus()){
		case JobDisplayData::StatusInactiveOrNeverBuilt:
			m_rectColor = Qt::gray;
		break;

		case JobDisplayData::StatusLastBuildSuccessful:
			m_rectColor = Qt::darkGreen;
		break;

		case JobDisplayData::StatusLastBuildSuccessfulButUnstable:
			m_rectColor = Qt::darkYellow;
		break;

		case JobDisplayData::StatusLastBuildFailed:
			m_rectColor = Qt::darkRed;
		break;
	}

}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void JobGraphicsItem::rectChanged(){
	m_nameItem->setRect(boundingRect());
}
//------------------------------------------------------------------------------
