//------------------------------------------------------------------------------
#ifndef JOBGRAPHICSITEM_H
#define JOBGRAPHICSITEM_H
//------------------------------------------------------------------------------
#include "jobdisplaydata.h"

#include <QGraphicsObject>
//------------------------------------------------------------------------------
class QSizeF;
class QGraphicsPixmapItem;
class WeatherGraphicsItem;
class AutoResizingTextItem;
//------------------------------------------------------------------------------
class JobGraphicsItem : public QGraphicsObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit JobGraphicsItem(QGraphicsItem *parent = 0);
		~JobGraphicsItem();

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		const QString & getName() const;
		bool isRunning() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setSize(qreal w, qreal h);
		void setSize(const QSizeF & size);
		void setRect(const QRectF &rect);

		void setName(const QString &name);
		void update(const JobDisplayData& data);
		void updateProgress();

//------------------------------------------------------------------------------
// Private
//------------------------------------------------------------------------------
	private:
		void updateLayout();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QString	m_name;
		QSizeF	m_size;
		QColor	m_rectColor;

		AutoResizingTextItem	*m_nameItem;
		AutoResizingTextItem	*m_estEndTimeItem;
		AutoResizingTextItem	*m_descriptionItem;
		WeatherGraphicsItem	*m_weatherItem;

		bool			m_running;
		qreal			m_progressFactor;
		qint64		m_buildStartTime;
		qint64		m_buildEstEndTime;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JOBGRAPHICSITEM_H
//------------------------------------------------------------------------------
