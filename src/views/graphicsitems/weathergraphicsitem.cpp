//------------------------------------------------------------------------------
#include "weathergraphicsitem.h"

#include <QGraphicsPixmapItem>
#include <QPainter>

//#define DEBUG_RECT 1
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
WeatherGraphicsItem::WeatherGraphicsItem(QGraphicsItem *parent):QGraphicsObject(parent),
	m_weatherIcon(NoIcon),
	m_size(),
	m_weatherPixmap(){
	m_weatherItem = new QGraphicsPixmapItem(this); // Deleted with parent (this)
}
//------------------------------------------------------------------------------
WeatherGraphicsItem::~WeatherGraphicsItem(){ }
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
QRectF WeatherGraphicsItem::boundingRect() const{
	return QRectF(0,0,m_size.width(),m_size.height());
}
//------------------------------------------------------------------------------
void WeatherGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
#if DEBUG_RECT
	painter->save();
	painter->setPen(Qt::red);
	painter->drawRect(boundingRect());
	painter->restore();
#endif
}
//------------------------------------------------------------------------------
WeatherGraphicsItem::eWeatherIcon WeatherGraphicsItem::weatherIcon() const{
	return m_weatherIcon;
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void WeatherGraphicsItem::setSize(qreal w, qreal h){
	m_size = QSizeF(w,h);
	updateIcon();
}
//------------------------------------------------------------------------------
void WeatherGraphicsItem::setSize(const QSizeF & size){
	m_size = size;
	updateIcon();
}
//------------------------------------------------------------------------------
void WeatherGraphicsItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	setSize(rect.width(), rect.height());
}
//------------------------------------------------------------------------------
void WeatherGraphicsItem::setWeatherIcon(WeatherGraphicsItem::eWeatherIcon icon){
	m_weatherIcon = icon;

	switch(icon){
		case NoIcon:			m_weatherPixmap =  QPixmap(); break;
		case Sunny:				m_weatherPixmap.load(":/weathericons/Sunny"); break;
		case PartlyCloudy:	m_weatherPixmap.load(":/weathericons/PartlyCloudy"); break;
		case Cloudy:			m_weatherPixmap.load(":/weathericons/Cloudy"); break;
		case Rain:				m_weatherPixmap.load(":/weathericons/Rain"); break;
		case Thunder:			m_weatherPixmap.load(":/weathericons/Thunder"); break;
	}

	updateIcon();
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void WeatherGraphicsItem::updateIcon(){
	if(m_weatherPixmap.isNull()){
		m_weatherItem->setPixmap(QPixmap());
		m_weatherItem->setVisible(false);
		return;
	}
	if(m_size.isEmpty() || m_size.isNull()){
		m_weatherItem->setPixmap(QPixmap());
		m_weatherItem->setVisible(false);
		return;
	}

	const QRectF rect(boundingRect());

	// Scale pixmap to fit in our rect
	QPixmap scaledPixmap(m_weatherPixmap.scaled(m_size.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

	// Add to item
	m_weatherItem->setPixmap(scaledPixmap);

	// Then center item in rect
	m_weatherItem->setPos(
		(rect.width()-scaledPixmap.width())/2,
		(rect.height()-scaledPixmap.height())/2
	);

	if(!m_weatherItem->isVisible())
		m_weatherItem->setVisible(true);
}
//------------------------------------------------------------------------------
