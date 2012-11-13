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
	m_nameItem(0),
	m_estEndTimeItem(0),
	m_running(false),
	m_progressFactor(0),
	m_buildStartTime(0),
	m_buildEstEndTime(0)
{
	m_nameItem = new AutoResizingTextItem(this);
	m_nameItem->setFont(QFont("Arial")); // TODO - Manage in prefs
	m_nameItem->setPen(QPen(Qt::white));

	m_estEndTimeItem = new AutoResizingTextItem(this);
	m_estEndTimeItem->setFont(QFont("Arial")); // TODO - Manage in prefs
	m_estEndTimeItem->setPen(QPen(Qt::white));
}
//------------------------------------------------------------------------------
JobGraphicsItem::~JobGraphicsItem(){
	if(m_nameItem)
		m_nameItem->deleteLater();

	if(m_estEndTimeItem)
		m_estEndTimeItem->deleteLater();
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
	painter->drawRoundedRect(rect, 10, 10);

	if(m_running){
		QRectF progressRect(rect);
		progressRect.setWidth(progressRect.width()*m_progressFactor);
		painter->setBrush(m_rectColor.lighter());
		painter->drawRoundedRect(progressRect, 10, 10);
	}

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
bool JobGraphicsItem::isRunning() const{
	return m_running;
}
//------------------------------------------------------------------------------
void JobGraphicsItem::update(const JobDisplayData& data){
	switch(data.getStatus()){
		case JobDisplayData::StatusInactiveOrNeverBuilt:
			m_rectColor = Qt::darkGray;
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

	m_running = data.isRunning();
	m_estEndTimeItem->setVisible(m_running);

	if(m_running){
		m_buildStartTime = data.getStartTime();
		m_buildEstEndTime	= m_buildStartTime + data.getEstimatedDuration();
		m_estEndTimeItem->setText(QDateTime::fromTime_t(m_buildEstEndTime).time().toString(QLocale().timeFormat(QLocale::ShortFormat)));
	}
	else
		m_progressFactor = 0;

	updateProgress();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::updateProgress(){
	if(m_running){
		time_t now = QDateTime::currentDateTime().toTime_t();

		if(now < m_buildStartTime){
			//qWarning()<<"JobGraphicsItem::update("<<data.getName()<<") - Current time is smaller than start time, maybe computers are out of sync (Now : "<<now<<" - Start time : "<<m_buildStartTime<<")";
			m_progressFactor = 0;
		}
		else if(now > m_buildEstEndTime || (m_buildStartTime >= m_buildEstEndTime)){
			m_progressFactor = 1;
		}
		else{
			qreal range = m_buildEstEndTime-m_buildStartTime;
			qreal done = now-m_buildStartTime;
			m_progressFactor = (done/range);
		}
		//qDebug()<<"JobGraphicsItem::update("<<m_nameItem->text()<<") - Started : "<<m_buildStartTime<<" - Should end : "<<m_buildEstEndTime<<" - Now : "<<now<<" - Progression : "<<m_progressFactor;
	}
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void JobGraphicsItem::rectChanged(){
	QRectF jobRect(boundingRect());

	// Name rect, 60% center of job rect
	QRectF nameRect(jobRect.x() + jobRect.width()*0.2, jobRect.y(), jobRect.width()*0.6, jobRect.height());
	m_nameItem->setRect(nameRect);

	QRectF estEndRect(jobRect.x() + jobRect.width()*0.8, jobRect.y() + jobRect.height()*0.2, jobRect.width()*0.2, jobRect.height()*0.6);
	m_estEndTimeItem->setRect(estEndRect);
}
//------------------------------------------------------------------------------
