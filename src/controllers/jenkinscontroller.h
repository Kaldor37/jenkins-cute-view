//------------------------------------------------------------------------------
#ifndef JENKINSCONTROLLER_H
#define JENKINSCONTROLLER_H
//------------------------------------------------------------------------------
#include <QObject>

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
		void status_changed(QString status, int timeout=0);
//------------------------------------------------------------------------------
	public slots:
		void control(MainWindow *wnd);
		void prefs_APIUpdateIntervalChanged(uint value);
		void start();

		void selectedViewDataUpdated();
//------------------------------------------------------------------------------
	private slots:
		void timerTimeout();
//------------------------------------------------------------------------------
	private:
		QTimer					*m_updateTimer;
		JenkinsXMLAPIModel	*m_XMLAPIModel;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JENKINSCONTROLLER_H
//------------------------------------------------------------------------------
