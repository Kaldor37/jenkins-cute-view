//------------------------------------------------------------------------------
#ifndef VIEWMODEL_H
#define VIEWMODEL_H
//------------------------------------------------------------------------------
#include <QString>
#include <QObject>
#include <QVector>
#include <QNetworkReply>
//------------------------------------------------------------------------------
class JobModel;
//------------------------------------------------------------------------------
class ViewModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructors/Destructor
//------------------------------------------------------------------------------
	private:
		ViewModel() = delete;
		ViewModel(const ViewModel &) = delete;
		ViewModel & operator=(const ViewModel &) = delete;

	public:
		explicit ViewModel(const QString &name, const QString &url, QObject *parent=nullptr);
		~ViewModel();

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
	public:
		typedef QList<JobModel*>				JobsList;
		typedef QMap<QString, JobModel*>		JobsMap;

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void jobsListLoaded();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setName(const QString &name);
		void setUrl(const QString &url);
		void loadJobs();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		const QString &	getName() const;
		const QString &	getUrl() const;
		bool					isJobsListLoaded() const;
		JobsList				getJobs() const;

		void http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void job_loaded();
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void clearJobs();
//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QString			m_name;
		QString			m_url;
		bool			m_jobsListLoaded;
		JobsMap			m_jobs;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // VIEWMODEL_H
//------------------------------------------------------------------------------
