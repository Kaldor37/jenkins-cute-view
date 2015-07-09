//------------------------------------------------------------------------------
#ifndef JENKINSCONTROLLER_H
#define JENKINSCONTROLLER_H
//------------------------------------------------------------------------------
#include "views/jobdisplaydata.h"
#include "views/jenkinsgraphicsview.h"

#include <QObject>
#include <QList>
#include <QColor>
#include <QVector>

class MainWindow;
class QTimer;
class JenkinsXMLAPIModel;
class NodesListModel;
//------------------------------------------------------------------------------
class JenkinsController : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
	public:
		explicit JenkinsController(QObject *parent=nullptr);
		~JenkinsController();
//------------------------------------------------------------------------------
	signals:
		void jobs_updated(QList<JobDisplayData> jobs);
		void nodes_updated(QVector<JenkinsGraphicsView::NodeData> nodes);

//------------------------------------------------------------------------------
	public slots:
		void control(MainWindow *wnd);
		void prefs_APIUpdateIntervalChanged(uint value);
		void start();

		void selectedViewDataUpdated();

	private slots:
		void updateNodesList();
//------------------------------------------------------------------------------
	private:
		QTimer					*m_updateTimer;
		JenkinsXMLAPIModel	*m_XMLAPIModel;
		NodesListModel			*m_nodesModel;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JENKINSCONTROLLER_H
//------------------------------------------------------------------------------
