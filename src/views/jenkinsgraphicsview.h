//------------------------------------------------------------------------------
#ifndef JENKINSGRAPHICSVIEW_H
#define JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
#include "jobdisplaydata.h"
#include "graphicsitems/messagegraphicsitem.h"

#include <QGraphicsView>
#include <QMap>
#include <QMutex>
//------------------------------------------------------------------------------
class JenkinsGraphicsScene;
class JobGraphicsItem;
class NodeGraphicsItem;
class QTimer;
class AutoResizingTextItem;
class QMenu;
class QAction;
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
// Signals
//------------------------------------------------------------------------------
	signals:
		void fullScreenTriggered();
		void preferencesTriggered();
		void quitTriggered();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void updateJobs(const QList<JobDisplayData> &);
		void updateNodes(const QVector<QString> &nodeNames, const QVector<QColor> &nodeColors);

		void displayMessage(const QString & msg, MessageGraphicsItem::eMessageType type=MessageGraphicsItem::Normal);
		void displayWarning(const QString & msg);
		void displayError(const QString & msg);

		void fullscreenModeChanged(bool enabled);
		void setColumns(uint value);
		void setShowNodes(bool value);

//------------------------------------------------------------------------------
// Protected functions
//------------------------------------------------------------------------------
	protected:
		void resizeEvent(QResizeEvent *event);
		void contextMenuEvent(QContextMenuEvent *event);

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void updateDisplay();
		void initContextMenu();

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
		typedef QMap<QString,NodeGraphicsItem*> NodesItems;

		JenkinsGraphicsScene	*m_scene;
		JobsItems				m_jobItems;
		NodesItems				m_nodeItems;
		QTimer					*m_progressUpdateTimer;
		MessageGraphicsItem	*m_messageItem;

		QMenu						*m_contextMenu;
		QAction					*m_fullscreenAction;
		QAction					*m_preferencesAction;
		QAction					*m_quitAction;

		uint						m_columns;
		bool						m_showNodes;
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
