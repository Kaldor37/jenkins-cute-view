//------------------------------------------------------------------------------
#ifndef JOBGRAPHICSITEM_H
#define JOBGRAPHICSITEM_H
//------------------------------------------------------------------------------
#include "views/jobdisplaydata.h"

#include <QGraphicsObject>
//------------------------------------------------------------------------------
class QSizeF;
class QGraphicsPixmapItem;
class WeatherGraphicsItem;
class AutoResizingTextItem;
class JenkinsGraphicsView;
//------------------------------------------------------------------------------
class JobGraphicsItem : public QGraphicsObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit JobGraphicsItem(JenkinsGraphicsView *view, QGraphicsItem *parent = 0);
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

		void setShowBuildNumber(bool val);
		void setShowWeather(bool val);
		void setShowLastBuildDesc(bool val);
		void setShowEstEndTime(bool val);
		void setShowPositionInQueue(bool val);

//------------------------------------------------------------------------------
// Private
//------------------------------------------------------------------------------
	private:
		void updateLayout();
		void updateName();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		JenkinsGraphicsView *m_view;

		QString	m_name;
		uint		m_lastBuildNum;
		QSizeF	m_size;
		QColor	m_rectColor;

		AutoResizingTextItem	*m_nameItem;
		AutoResizingTextItem	*m_estEndTimeItem;
		AutoResizingTextItem	*m_descriptionItem;
		WeatherGraphicsItem	*m_weatherItem;
		AutoResizingTextItem	*m_positionInQueueItem;

		bool			m_running;
		qreal			m_progressFactor;
		qint64		m_buildStartTime;
		qint64		m_buildEstEndTime;
		uint			m_positionInQueue;

		bool m_showBuildNumber;
		bool m_showWeather;
		bool m_showLastBuildDesc;
		bool m_showEstEndTime;
		bool m_showPositionInQueue;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JOBGRAPHICSITEM_H
//------------------------------------------------------------------------------
