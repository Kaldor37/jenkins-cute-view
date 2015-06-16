//------------------------------------------------------------------------------
#ifndef JOBMODEL_H
#define JOBMODEL_H
//------------------------------------------------------------------------------
#include <QString>
#include <QObject>
#include <QNetworkReply>
//------------------------------------------------------------------------------
class BuildModel;
class QDomDocument;
//------------------------------------------------------------------------------
class JobModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructors/Destructor
//------------------------------------------------------------------------------
	private:
		JobModel();
		JobModel(const JobModel &src);
		JobModel & operator=(const JobModel &src);

	public:
		explicit JobModel(const QString &name, const QString &url, QObject *parent=0);
		~JobModel();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		bool isLoaded() const;
		const BuildModel * getLastBuild() const;
		const BuildModel * getLastCompletedBuild() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	signals:
		void loaded();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void load();

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);
		void lastBuild_loaded();
		void lastCompletedBuild_loaded();

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void parseJob(const QDomDocument &doc);

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		bool m_lastBuildLoaded;
		bool m_lastCompletedBuildLoaded;
		BuildModel * m_lastBuild = nullptr;
		BuildModel * m_lastCompletedBuild = nullptr;

//------------------------------------------------------------------------------
// Members declaration
//------------------------------------------------------------------------------
#define JOB_DECLARE_MEMBER(Type, Member) \
	public: \
		const Type & get##Member() const; \
	public slots: \
		void set##Member(const Type & Member); \
	private: \
		Type m_##Member;
//------------------------------------------------------------------------------
// General
JOB_DECLARE_MEMBER(QString,	Name)
JOB_DECLARE_MEMBER(QString,	Url)
JOB_DECLARE_MEMBER(QString,	Description)
JOB_DECLARE_MEMBER(QString,	DisplayName)
JOB_DECLARE_MEMBER(bool,		Buildable)
JOB_DECLARE_MEMBER(QString,	Color)
JOB_DECLARE_MEMBER(bool,		InQueue)
JOB_DECLARE_MEMBER(bool,		KeepDependencies)

// Health
JOB_DECLARE_MEMBER(QString,	HealthDescription)
JOB_DECLARE_MEMBER(int,			HealthScore)

// Builds
JOB_DECLARE_MEMBER(uint,		NextBuildNumber)
JOB_DECLARE_MEMBER(bool,		ConcurrentBuild)
//------------------------------------------------------------------------------
#undef JOB_DECLARE_MEMBER
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JOBMODEL_H
//------------------------------------------------------------------------------
