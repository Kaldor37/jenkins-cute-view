//------------------------------------------------------------------------------
#ifndef JENKINSAPIMODEL_H
#define JENKINSAPIMODEL_H
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
//------------------------------------------------------------------------------
class JenkinsAPIModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit JenkinsAPIModel(QObject *parent=nullptr);
		~JenkinsAPIModel();

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
	public:
		typedef QVector<ViewModel*> ViewsList;

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void viewsNamesUpdated(QStringList viewsNames, QString selectedViewName);
		void selectedViewLoaded();
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
		 * List of jobs in queue
		 */
		const QVector<QString> & jobsQueue() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setJenkinsUrl(const QString &url);
		void setSelectedView(const QString &name);
		void loadViews();
		void loadSelectedView();
		void loadJobsQueue();

		void viewsList_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);
		void jobsQueue_httpFinished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void selectedView_jobsListLoaded();

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void clearViews();

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

		QVector<QString>	m_jobsQueue;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JENKINSAPIMODEL_H
//------------------------------------------------------------------------------
