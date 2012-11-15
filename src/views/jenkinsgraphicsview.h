//------------------------------------------------------------------------------
#ifndef JENKINSGRAPHICSVIEW_H
#define JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
#include "jobdisplaydata.h"
#include "messagegraphicsitem.h"

#include <QGraphicsView>
#include <QMap>
#include <QMutex>
//------------------------------------------------------------------------------
class JenkinsGraphicsScene;
class JobGraphicsItem;
class QTimer;
class AutoResizingTextItem;
//------------------------------------------------------------------------------
// Graphics View
//------------------------------------------------------------------------------
class JenkinsGraphicsView : public QGraphicsView {
		Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit JenkinsGraphicsView(QWidget *parent = 0);
		~JenkinsGraphicsView();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void updateJobs(const QList<JobDisplayData> &);

		void displayMessage(const QString & msg, MessageGraphicsItem::eMessageType type=MessageGraphicsItem::Normal);
		void displayWarning(const QString & msg);
		void displayError(const QString & msg);

//------------------------------------------------------------------------------
// Protected functions
//------------------------------------------------------------------------------
	protected:
		void resizeEvent(QResizeEvent *event);

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void updateDisplay();

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void progressTimer_timeout();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		typedef QMap<QString,JobGraphicsItem*> JobsItems;

		JenkinsGraphicsScene	*m_scene;
		JobsItems				m_jobItems;
		QMutex					m_jobsMutex;
		QTimer					*m_progressUpdateTimer;
		MessageGraphicsItem	*m_messageItem;

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
	private:
		const static int jobsMargin = 4;
		const static int jobsProgressUpdateTimer = 100;

//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
// Graphics Scene
//------------------------------------------------------------------------------
class JenkinsGraphicsScene : public QGraphicsScene {
		Q_OBJECT

	public:
		explicit JenkinsGraphicsScene(QWidget *parent = 0);
		~JenkinsGraphicsScene();
};
//------------------------------------------------------------------------------
#endif // JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
