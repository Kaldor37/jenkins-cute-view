//------------------------------------------------------------------------------
#ifndef JENKINSXMLAPIMODEL_H
#define JENKINSXMLAPIMODEL_H
//------------------------------------------------------------------------------
#include "models/jobmodel.h"
#include "models/viewmodel.h"

#include <QObject>
#include <QNetworkReply>
#include <QVector>
#include <QStringList>
//------------------------------------------------------------------------------
class QString;
class HttpGetter;
class JobModel;
class ViewModel;
class QDomDocument;
//------------------------------------------------------------------------------
class JenkinsXMLAPIModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit JenkinsXMLAPIModel(QObject *parent=0);
		~JenkinsXMLAPIModel();

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

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);
		void selectedView_jobsListLoaded();
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void parseViews(const QDomDocument &doc);
		void clearViews();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QString			m_jenkinsUrl;
		QString			m_selectedViewName;

		bool				m_viewsListLoaded;
		ViewsList		m_views;
		ViewModel *		m_primaryView;
		ViewModel *		m_selectedView;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JENKINSXMLAPIMODEL_H
//------------------------------------------------------------------------------
