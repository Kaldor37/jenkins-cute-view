//------------------------------------------------------------------------------
#include "weathergraphicsitem.h"
#include "preferences.h"
#include <QGraphicsPixmapItem>
#include <QPainter>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
WeatherGraphicsItem::WeatherGraphicsItem(QGraphicsItem *parent):QGraphicsObject(parent),
	m_weatherIcon(WeatherIcon::NoIcon),
	m_weatherIconsTheme("weather-icons-theme0") // Default weather icons theme
{
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
WeatherGraphicsItem::WeatherIcon WeatherGraphicsItem::weatherIcon() const{
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
void WeatherGraphicsItem::setWeatherIcon(WeatherGraphicsItem::WeatherIcon icon){
	m_weatherIcon = icon;
	loadPixmap();
	updateIcon();
}
//------------------------------------------------------------------------------
void WeatherGraphicsItem::setWeatherTheme(const QString &theme){
	// Theme doesn't change
	if(theme == m_weatherIconsTheme)
		return;

	m_weatherIconsTheme = theme;
	loadPixmap();
	updateIcon();
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void WeatherGraphicsItem::updateIcon(){
	if(
		m_weatherPixmap.isNull() ||
		m_size.isEmpty() || m_size.isNull() ||
		m_weatherIcon == WeatherIcon::NoIcon
	){
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
void WeatherGraphicsItem::loadPixmap(){
	Q_ASSERT(!m_weatherIconsTheme.isEmpty());

	switch(m_weatherIcon){
		case WeatherIcon::NoIcon:			m_weatherPixmap =  QPixmap(); break;
		case WeatherIcon::Sunny:			m_weatherPixmap.load(":/"+m_weatherIconsTheme+"/Sunny"); break;
		case WeatherIcon::PartlyCloudy:	m_weatherPixmap.load(":/"+m_weatherIconsTheme+"/PartlyCloudy"); break;
		case WeatherIcon::Cloudy:			m_weatherPixmap.load(":/"+m_weatherIconsTheme+"/Cloudy"); break;
		case WeatherIcon::Rain:				m_weatherPixmap.load(":/"+m_weatherIconsTheme+"/Rain"); break;
		case WeatherIcon::Thunder:			m_weatherPixmap.load(":/"+m_weatherIconsTheme+"/Thunder"); break;
	}
}
//------------------------------------------------------------------------------
