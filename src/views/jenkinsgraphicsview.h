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
		explicit JenkinsGraphicsView(QWidget *parent = nullptr);
		~JenkinsGraphicsView();

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void fullScreenTriggered();
		void preferencesTriggered();
		void quitTriggered();
		void contextMenuShown();
		void contextMenuHidden();

//------------------------------------------------------------------------------
// Public typedefs
//------------------------------------------------------------------------------
	public:
		enum class NodeStatus {
			Idle,
			Working,
			TemporarilyOffline,
			Offline
		};

		struct NodeData {
			QString name;
			NodeStatus status = NodeStatus::Offline;
			uint responseTime = 0;
		};

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void updateJobs(const QList<JobDisplayData> &);
		void updateNodes(const QVector<NodeData> &nodes);

		void displayMessage(const QString & msg, MessageGraphicsItem::MessageType type);
		void displayMessage(const QString & msg);
		void displayWarning(const QString & msg);
		void displayError(const QString & msg);

		void fullscreenModeChanged(bool enabled);
		void setColumns(uint value);
		void setShowNodes(bool value);
		void setJobsMargin(uint value);

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

		uint						m_jobsMargin;
//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
	private:
		const static int jobsProgressUpdateTimer = 200;

//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
// Graphics Scene
//------------------------------------------------------------------------------
class JenkinsGraphicsScene : public QGraphicsScene {
		Q_OBJECT

	public:
		explicit JenkinsGraphicsScene(QWidget *parent = nullptr);
		~JenkinsGraphicsScene();
};
//------------------------------------------------------------------------------
#endif // JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
