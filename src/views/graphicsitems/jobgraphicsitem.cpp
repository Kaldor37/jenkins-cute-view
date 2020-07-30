//------------------------------------------------------------------------------
#include "jobgraphicsitem.h"
#include "autoresizingtextitem.h"
#include "weathergraphicsitem.h"
#include "preferences.h"
#include "application.h"
#include "views/jenkinsgraphicsview.h"

#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QDateTime>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JobGraphicsItem::JobGraphicsItem(JenkinsGraphicsView *view, QGraphicsItem *parent/* = nullptr*/):QGraphicsObject(parent),
	m_view(view),
	m_lastBuildNum(0),
	m_nameItem(new AutoResizingTextItem(this)),
	m_estEndTimeItem(new AutoResizingTextItem(this)),
	m_descriptionItem(new AutoResizingTextItem(this)),
	m_weatherItem(new WeatherGraphicsItem(this)),
	m_positionInQueueItem(new AutoResizingTextItem(this)),
	m_jobsReportItem(new AutoResizingTextItem(this)),
	m_running(false),
	m_progressFactor(0),
	m_buildStartTime(0),
	m_buildEstEndTime(0),
	m_showBuildNumber(Prefs.getShowBuildNumber()),
	m_showWeather(Prefs.getShowWeatherIcon()),
	m_showLastBuildDesc(Prefs.getShowLastBuildDescription()),
	m_showEstEndTime(Prefs.getShowEstimatedEndTime()),
	m_showPositionInQueue(Prefs.getShowPositionInQueue())
{
	Preferences *prefs = &Prefs;

	m_nameItem->setFont(QFont(prefs->getFont(), -1, QFont::Bold));
	m_nameItem->setPen(QPen(Qt::white));
	m_nameItem->setShadowed(true);
	m_nameItem->setTextFlags(prefs->getJobsNameDescAlignFlags());

	m_estEndTimeItem->setFont(QFont(prefs->getFont()));
	m_estEndTimeItem->setPen(QPen(Qt::white));
	m_estEndTimeItem->setShadowed(true);

	m_descriptionItem->setFont(QFont(prefs->getFont(), -1, QFont::DemiBold));
	m_descriptionItem->setPen(QPen(Qt::white));
	m_descriptionItem->setVisible(false);
	m_descriptionItem->setTextFlags(prefs->getJobsNameDescAlignFlags());

	m_weatherItem->setWeatherTheme(prefs->getWeatherIconsTheme());

	m_positionInQueueItem->setFont(QFont(prefs->getFont(), -1, QFont::Bold));
	m_positionInQueueItem->setPen(QPen(Qt::white));
	m_positionInQueueItem->setShadowed(true);

	m_jobsReportItem->setFont(QFont(prefs->getFont(), -1, QFont::Bold));
	m_jobsReportItem->setPen(QPen(Qt::white));
	m_jobsReportItem->setShadowed(true);

	// Prefs changes
	QObject::connect(prefs, &Preferences::sigShowBuildNumberChanged, this, &JobGraphicsItem::setShowBuildNumber);
	QObject::connect(prefs, &Preferences::sigShowEstimatedEndTimeChanged, this, &JobGraphicsItem::setShowEstEndTime);
	QObject::connect(prefs, &Preferences::sigShowLastBuildDescriptionChanged, this, &JobGraphicsItem::setShowLastBuildDesc);
	QObject::connect(prefs, &Preferences::sigShowWeatherIconChanged, this, &JobGraphicsItem::setShowWeather);
	QObject::connect(prefs, &Preferences::sigShowPositionInQueueChanged, this, &JobGraphicsItem::setShowPositionInQueue);
	QObject::connect(prefs, &Preferences::sigJobsNameDescAlignFlagsChanged, m_nameItem, &AutoResizingTextItem::setTextFlags);
	QObject::connect(prefs, &Preferences::sigFontChanged, m_nameItem, &AutoResizingTextItem::setFontFamily);
	QObject::connect(prefs, &Preferences::sigFontChanged, m_estEndTimeItem, &AutoResizingTextItem::setFontFamily);
	QObject::connect(prefs, &Preferences::sigJobsNameDescAlignFlagsChanged, m_descriptionItem, &AutoResizingTextItem::setTextFlags);
	QObject::connect(prefs, &Preferences::sigFontChanged, m_descriptionItem, &AutoResizingTextItem::setFontFamily);
	QObject::connect(prefs, &Preferences::sigWeatherIconsThemeChanged, m_weatherItem, &WeatherGraphicsItem::setWeatherTheme);
	QObject::connect(prefs, &Preferences::sigFontChanged, m_positionInQueueItem, &AutoResizingTextItem::setFontFamily);
	QObject::connect(prefs, &Preferences::sigFontChanged, m_jobsReportItem, &AutoResizingTextItem::setFontFamily);
}
//------------------------------------------------------------------------------
JobGraphicsItem::~JobGraphicsItem(){ }
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
		case JobDisplayData::JobStatus::NeverBuilt:
		case JobDisplayData::JobStatus::InactiveOrAborted:
			m_rectColor = m_view->property("InactiveOrNeverBuilt").value<QColor>();
		break;

		case JobDisplayData::JobStatus::LastBuildSuccessful:
			m_rectColor = m_view->property("LastBuildSuccess").value<QColor>();
		break;

		case JobDisplayData::JobStatus::LastBuildSuccessfulButUnstable:
			m_rectColor = m_view->property("LastBuildUnstable").value<QColor>();
		break;

		case JobDisplayData::JobStatus::LastBuildFailed:
			m_rectColor = m_view->property("LastBuildFailed").value<QColor>();
		break;
	}

	m_lastBuildNum = data.getLastBuildNum();
	m_name = data.getName();

	m_running = data.isRunning();
	m_estEndTimeItem->setVisible(m_running && m_showEstEndTime);

	if(m_running){
		m_buildStartTime = data.getStartTime();
		m_buildEstEndTime	= m_buildStartTime + data.getEstimatedDuration();
		m_estEndTimeItem->setText(
			QDateTime::fromMSecsSinceEpoch(m_buildEstEndTime).time().toString(QLocale().timeFormat(QLocale::ShortFormat))
		);
	}
	else
		m_progressFactor = 0;

	// Description
	if(data.getType() == JobDisplayData::JobType::Freestyle){
		m_descriptionItem->setText(data.getLastBuildDesc());
		m_descriptionItem->setVisible(!m_descriptionItem->text().isEmpty() && m_showLastBuildDesc);
	}

	// Weather
	if(data.getStatus() != JobDisplayData::JobStatus::NeverBuilt){
		uint stability = data.getStability();
		if(stability > 80)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::WeatherIcon::Sunny);
		else if(stability <= 80  && stability > 60)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::WeatherIcon::PartlyCloudy);
		else if(stability <= 60  && stability > 40)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::WeatherIcon::Cloudy);
		else if(stability <= 40  && stability > 20)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::WeatherIcon::Rain);
		else if(stability <= 20)
			m_weatherItem->setWeatherIcon(WeatherGraphicsItem::WeatherIcon::Thunder);

		m_weatherItem->setVisible(m_showWeather);
	}
	else{
		m_weatherItem->setVisible(false);
	}

	if(data.getType() == JobDisplayData::JobType::Freestyle){
		// Position in queue
		m_positionInQueue = data.getPositionInQueue();
		if(m_positionInQueue > 0 && m_showPositionInQueue && (!m_running || !m_showEstEndTime)){
			m_positionInQueueItem->setText(QString("%1").arg(m_positionInQueue));
			m_positionInQueueItem->setVisible(true);
		}
		else
			m_positionInQueueItem->setVisible(false);

		m_jobsReportItem->setVisible(false);
	}
	else if(data.getType() == JobDisplayData::JobType::MultibranchPipeline){
		if(m_showJobsReport && (!m_running || !m_showEstEndTime)){
			m_jobsReportItem->setText(QString("%1/%2").arg(data.getSucceededJobs()).arg(data.getTotalJobs()));
			m_jobsReportItem->setVisible(true);
		}
		else{
			m_jobsReportItem->setVisible(false);
		}
		m_positionInQueueItem->setVisible(false);
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
	if(!m_running)
		return;

	qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
	if(now < m_buildStartTime){
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
	QGraphicsItem::update();
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
	m_positionInQueueItem->setVisible(m_showPositionInQueue && m_positionInQueue > 0 && (!m_running || !val));
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setShowPositionInQueue(bool val){
	m_showPositionInQueue = val;
	m_positionInQueueItem->setVisible(val && m_positionInQueue > 0 && (!m_running || !m_showEstEndTime));
	updateLayout();
}
//------------------------------------------------------------------------------
void JobGraphicsItem::setShowJobsReport(bool val){
	m_showJobsReport = val;
	m_jobsReportItem->setVisible(val && (!m_running || !m_showEstEndTime));
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
		descRect.setY(jobY + jobH * 0.6);
		descRect.setHeight(jobH * 0.4);
		nameRect.setY(jobY);
	}
	else
		nameRect.setY(jobY + jobH*0.15);

	nameRect.setHeight(jobH * 0.6);

	// Estimated end time and weather on both sides
	if(m_estEndTimeItem->isVisible() || m_weatherItem->isVisible()){
		// Estimated end time 20% right
		estEndRect = QRectF(jobX + jobW*0.85,	jobY + jobH*0.2,	jobW*0.14,	jobH*0.6);
		// Weather 20% left
		weatherRect = QRectF(jobX,					jobY + jobH*0.1,	jobW*0.15,	jobH*0.8);
		// Name rect middle
		nameRect.setX(jobX + jobW*0.15);
		nameRect.setWidth(jobW*0.7);
	}
	else{
		// Name rect full width
		nameRect.setX(jobX + jobW*0.05);
		nameRect.setWidth(jobW*0.95);
	}

	descRect.setX(nameRect.x());
	descRect.setWidth(nameRect.width());

	m_nameItem->setRect(nameRect);
	m_descriptionItem->setRect(descRect);
	m_estEndTimeItem->setRect(estEndRect);
	m_weatherItem->setRect(weatherRect);
	m_positionInQueueItem->setRect(estEndRect);
	m_jobsReportItem->setRect(estEndRect);
}
//------------------------------------------------------------------------------
