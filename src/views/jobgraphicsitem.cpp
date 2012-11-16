//------------------------------------------------------------------------------
#include "views/jobgraphicsitem.h"
#include "autoresizingtextitem.h"
#include "weathergraphicsitem.h"
#include "preferences.h"

#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QDateTime>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JobGraphicsItem::JobGraphicsItem(QGraphicsItem *parent/* = 0*/):QGraphicsObject(parent),
	m_name(),
	m_lastBuildNum(0),
	m_nameItem(0),
	m_estEndTimeItem(0),
	m_descriptionItem(0),
	m_weatherItem(0),
	m_running(false),
	m_progressFactor(0),
	m_buildStartTime(0),
	m_buildEstEndTime(0),
	m_showBuildNumber(Prefs.getShowBuildNumber()),
	m_showWeather(Prefs.getShowWeatherIcon()),
	m_showLastBuildDesc(Prefs.getShowLastBuildDescription()),
	m_showEstEndTime(Prefs.getShowEstimatedEndTime())
{
	m_nameItem = new AutoResizingTextItem(this);
	m_nameItem->setFont(QFont("Arial", -1, QFont::Bold)); // TODO - Manage in prefs
	m_nameItem->setPen(QPen(Qt::white));

	m_estEndTimeItem = new AutoResizingTextItem(this);
	m_estEndTimeItem->setFont(QFont("Arial")); // TODO - Manage in prefs
	m_estEndTimeItem->setPen(QPen(Qt::white));

	m_descriptionItem = new AutoResizingTextItem(this);
	m_descriptionItem->setFont(QFont("Arial")); // TODO - Manage in prefs
	m_descriptionItem->setPen(QPen(Qt::white));
	m_descriptionItem->setVisible(false);

	m_weatherItem = new WeatherGraphicsItem(this);

	Preferences *prefs = &Prefs;
	connect(prefs, SIGNAL(sigShowBuildNumberChanged(bool)), SLOT(setShowBuildNumber(bool)));
	connect(prefs, SIGNAL(sigShowEstimatedEndTimeChanged(bool)), SLOT(setShowEstEndTime(bool)));
	connect(prefs, SIGNAL(sigShowLastBuildDescriptionChanged(bool)), SLOT(setShowLastBuildDesc(bool)));
	connect(prefs, SIGNAL(sigShowWeatherIconChanged(bool)), SLOT(setShowWeather(bool)));
}
//------------------------------------------------------------------------------
JobGraphicsItem::~JobGraphicsItem(){
	if(m_nameItem)
		m_nameItem->deleteLater();

	if(m_estEndTimeItem)
		m_estEndTimeItem->deleteLater();

	if(m_descriptionItem)
		m_descriptionItem->deleteLater();

	if(m_weatherItem)
		m_weatherItem->deleteLater();
}
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
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setSize(const QSizeF & size){
	m_size = size;
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	setSize(rect.width(), rect.height());
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setName(const QString &name){
	m_name = name;
	Q_ASSERT(m_nameItem);
	m_nameItem->setText(m_name);
}
//------------------------------------------------------------------------------
const QString & JobGraphicsItem::getName() const{
	return m_name;
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

	m_lastBuildNum = data.getLastBuildNum();
	m_name = data.getName();

	m_running = data.isRunning();
	m_estEndTimeItem->setVisible(m_running && m_showEstEndTime);

	if(m_running){
		m_buildStartTime = data.getStartTime();
		m_buildEstEndTime	= m_buildStartTime + data.getEstimatedDuration();

#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
		m_estEndTimeItem->setText(QDateTime::fromMSecsSinceEpoch(m_buildEstEndTime).time().toString(QLocale().timeFormat(QLocale::ShortFormat)));
#else
		m_estEndTimeItem->setText(QDateTime::fromTime_t(((uint)m_buildEstEndTime)/1000).time().toString(QLocale().timeFormat(QLocale::ShortFormat)));
#endif

	}
	else
		m_progressFactor = 0;

	// Description
	m_descriptionItem->setText(data.getLastBuildDesc());
	m_descriptionItem->setVisible(!m_descriptionItem->text().isEmpty() && m_showLastBuildDesc);

	// Weather
	if(data.getStatus() != JobDisplayData::StatusInactiveOrNeverBuilt){
		uint stability = data.getStability();
		if(stability > 80)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::Sunny);
		else if(stability <= 80  && stability > 60)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::PartlyCloudy);
		else if(stability <= 60  && stability > 40)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::Cloudy);
		else if(stability <= 40  && stability > 20)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::Rain);
		else if(stability <= 20)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::Thunder);

		m_weatherItem->setVisible(m_showWeather);
	}
	else{
		m_weatherItem->setVisible(false);
	}

	updateName();
	updateProgress();
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::updateName(){
	if(m_lastBuildNum > 0 && m_showBuildNumber)
		m_nameItem->setText(QString("%1 #%2").arg(m_name).arg(m_lastBuildNum));
	else
		m_nameItem->setText(m_name);
}
//------------------------------------------------------------------------------
void JobGraphicsItem::updateProgress(){
	if(m_running){
		qint64 now = 0;

#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
		now = QDateTime::currentDateTime().toMSecsSinceEpoch();
#else
		{
			QDateTime dt = QDateTime::currentDateTime();
			now = (((qint64)dt.toTime_t())*1000) + dt.time().msec();
		}
#endif

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
void JobGraphicsItem::setShowBuildNumber(bool val){
	m_showBuildNumber = val;
	updateName();
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setShowWeather(bool val){
	m_showWeather = val;
	m_weatherItem->setVisible(val);
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setShowLastBuildDesc(bool val){
	m_showLastBuildDesc = val;
	m_descriptionItem->setVisible(!m_descriptionItem->text().isEmpty() && val);
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setShowEstEndTime(bool val){
	m_showEstEndTime = val;
	m_estEndTimeItem->setVisible(val && m_running);
	updateLayout();
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void JobGraphicsItem::updateLayout(){
	QRectF jobRect(boundingRect());
	qreal jobX = jobRect.x();
	qreal jobY = jobRect.y();
	qreal jobW = jobRect.width();
	qreal jobH = jobRect.height();

	QRectF nameRect;
	QRectF descRect;
	QRectF estEndRect;
	QRectF weatherRect;

	// Job description under name
	if(m_descriptionItem->isVisible()){
		descRect.setY(jobY + jobH * 0.7);
		descRect.setHeight(jobH * 0.3);
		nameRect.setY(jobY);
	}
	else
		nameRect.setY(jobY + jobH*0.15);

	nameRect.setHeight(jobH * 0.7);

	// Estimated end time and weather on both sides
	if(m_estEndTimeItem->isVisible() || m_weatherItem->isVisible()){
		// Estimated end time 20% right
		estEndRect = QRectF(jobX + jobW*0.8, jobY + jobH*0.2, jobW*0.2, jobH*0.6);
		// Weather 20% left
		weatherRect = QRectF(jobX, jobY + jobH*0.1, jobW*0.2, jobH*0.8);
		// Name rect middle
		nameRect.setX(jobX + jobW*0.2);
		nameRect.setWidth(jobW*0.6);
	}
	else{
		// Name rect full width
		nameRect.setX(jobX);
		nameRect.setWidth(jobW);
	}

	descRect.setX(nameRect.x());
	descRect.setWidth(nameRect.width());

	m_nameItem->setRect(nameRect);
	m_descriptionItem->setRect(descRect);
	m_estEndTimeItem->setRect(estEndRect);
	m_weatherItem->setRect(weatherRect);
}
//------------------------------------------------------------------------------
