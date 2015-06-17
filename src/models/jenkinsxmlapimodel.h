//------------------------------------------------------------------------------
#ifndef JENKINSXMLAPIMODEL_H
#define JENKINSXMLAPIMODEL_H
//------------------------------------------------------------------------------
#include "models/jobmodel.h"

#include <QObject>
#include <QNetworkReply>
#include <QVector>
#include <QStringList>
//------------------------------------------------------------------------------
class QString;
class HttpGetter;
class JobModel;
class ViewModel;
class NodeModel;
class QDomDocument;
//------------------------------------------------------------------------------
class JenkinsXMLAPIModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit JenkinsXMLAPIModel(QObject *parent=nullptr);
		~JenkinsXMLAPIModel();

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
	public:
		typedef QVector<ViewModel*> ViewsList;
		typedef QVector<NodeModel*> NodesList;

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void viewsNamesUpdated(QStringList viewsNames, QString selectedViewName);
		void selectedViewLoaded();
		void nodesListLoaded();
		void jobsQueueLoaded();

		void message(QString);
		void warning(QString);
		void error(QString);

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		bool isViewsListLoaded() const;

		/**
		 * List of all views
		 */
		const ViewsList & views() const;

		/**
		 * Primary jenkins view
		 */
		const ViewModel * primaryView() const;

		/**
		 * Selected jenkins view
		 * If no view has been selected (or the selected view is invalid or
		 * unknown, returns primaryView)
		 * @see primaryView
		 */
		const ViewModel * selectedView() const;

		/**
		 * List of allnodes
		 */
		const NodesList & nodes() const;

		/**
		 * List of jobs in queue
		 */
		const QVector<QString> & jobsQueue() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		/**
		 * Sets Jenkins root URL
		 */
		void setJenkinsUrl(const QString &url);

		/**
		 * Sets selected Jenkins view
		 */
		void setSelectedView(const QString &name);

		/**
		 * Starts loading views list
		 */
		void loadViews();

		/**
		 * Loads selected view
		 */
		void loadSelectedView();

		/**
		 * Starts loading nodes list
		 */
		void loadNodes();

		/**
		 * Starts loading jobs queue
		 */
		void loadJobsQueue();

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void viewsList_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);
		void nodesList_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);
		void jobsQueue_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);

		void selectedView_jobsListLoaded();

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void parseViews(const QDomDocument &doc);
		void parseNodes(const QDomDocument &doc);
		void parseJobsQueue(const QDomDocument &doc);

		void clearViews();
		void clearNodes();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QString				m_jenkinsUrl;
		QString				m_selectedViewName;

		bool					m_viewsListLoaded;
		ViewsList			m_views;
		ViewModel *			m_primaryView;
		ViewModel *			m_selectedView;

		bool					m_nodesListLoaded;
		NodesList			m_nodes;

		bool					m_jobsQueueLoaded;
		QVector<QString>	m_jobsQueue;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JENKINSXMLAPIMODEL_H
//------------------------------------------------------------------------------
