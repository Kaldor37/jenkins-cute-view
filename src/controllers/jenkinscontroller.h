//------------------------------------------------------------------------------
#ifndef JENKINSCONTROLLER_H
#define JENKINSCONTROLLER_H
//------------------------------------------------------------------------------
#include "views/jobdisplaydata.h"

#include <QObject>
#include <QList>
#include <QColor>
#include <QVector>

class MainWindow;
class QTimer;
class JenkinsXMLAPIModel;
//------------------------------------------------------------------------------
class JenkinsController : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
	public:
		explicit JenkinsController(QObject *parent=0);
		~JenkinsController();
//------------------------------------------------------------------------------
	signals:
		void jobs_updated(QList<JobDisplayData> jobs);
		void nodes_updated(QVector<QString> nodesNames, QVector<QColor> nodesColors);

//------------------------------------------------------------------------------
	public slots:
		void control(MainWindow *wnd);
		void prefs_APIUpdateIntervalChanged(uint value);
		void start();

		void selectedViewDataUpdated();
		void nodesListUpdated();
//------------------------------------------------------------------------------
	private:
		QTimer					*m_updateTimer;
		JenkinsXMLAPIModel	*m_XMLAPIModel;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JENKINSCONTROLLER_H
//------------------------------------------------------------------------------
