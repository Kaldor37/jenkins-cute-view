//------------------------------------------------------------------------------
#ifndef NODESLISTMODEL_H
#define NODESLISTMODEL_H
//------------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QVector>
#include <QNetworkReply>
//------------------------------------------------------------------------------
class NodesListModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructors/Destructor
//------------------------------------------------------------------------------
	public:
		explicit NodesListModel(QObject *parent=nullptr);
		~NodesListModel();

	public:
		struct Node {
			QString	displayName;
			bool		idle = false;
			uint		numExecutors = 0;
			bool		offline = false;
			QString	offlineCauseReason;
			bool		temporarilyOffline = false;
			uint		ping = 0;
		};
		typedef QVector<Node> NodesList;

		const NodesList & nodes() const { return m_nodesList; }

	public slots:
		void update();

	signals:
		void updated();

	private:
		NodesListModel(const NodesListModel &src) = delete;
		NodesListModel & operator=(const NodesListModel &src) = delete;

	private slots:
		void httpFinished(const QString & data, QNetworkReply::NetworkError errorCode, const QString & errString);

	private:
		bool				m_updating = false;
		QVector<Node>	m_nodesList;
};
//------------------------------------------------------------------------------
#endif // NODESLISTMODEL_H
//------------------------------------------------------------------------------
