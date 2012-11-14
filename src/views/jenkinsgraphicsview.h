//------------------------------------------------------------------------------
#ifndef JENKINSGRAPHICSVIEW_H
#define JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
#include "jobdisplaydata.h"

#include <QGraphicsView>
#include <QMap>
//------------------------------------------------------------------------------
class JenkinsGraphicsScene;
class JobGraphicsItem;
class QTimer;
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
		JenkinsGraphicsScene *m_scene;

		typedef QMap<QString,JobGraphicsItem*> JobsItems;
		JobsItems m_jobItems;
		QTimer *m_progressUpdateTimer;

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
