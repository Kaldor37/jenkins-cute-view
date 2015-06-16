//------------------------------------------------------------------------------
#ifndef WEATHERGRAPHICSITEM_H
#define WEATHERGRAPHICSITEM_H
//------------------------------------------------------------------------------
#include <QGraphicsPixmapItem>
//------------------------------------------------------------------------------
class WeatherGraphicsItem : public QGraphicsObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Enum(s)
//------------------------------------------------------------------------------
	public:
		enum eWeatherIcon {
			NoIcon			= -1,
			Sunny				= 0,
			PartlyCloudy	= 1,
			Cloudy			= 2,
			Rain				= 3,
			Thunder			= 4
		};
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit WeatherGraphicsItem(QGraphicsItem *parent = nullptr);
		~WeatherGraphicsItem();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		eWeatherIcon weatherIcon() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setSize(qreal w, qreal h);
		void setSize(const QSizeF & size);
		void setRect(const QRectF &rect);
		void setWeatherIcon(eWeatherIcon);
		void setWeatherTheme(const QString &theme);

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void loadPixmap();
		void updateIcon();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		eWeatherIcon			m_weatherIcon;
		QSizeF					m_size;
		QPixmap					m_weatherPixmap;
		QGraphicsPixmapItem	*m_weatherItem;
		QString					m_weatherIconsTheme;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // WEATHERGRAPHICSITEM_H
//------------------------------------------------------------------------------
