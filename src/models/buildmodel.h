//------------------------------------------------------------------------------
#ifndef BUILDMODEL_H
#define BUILDMODEL_H
//------------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QNetworkReply>
//------------------------------------------------------------------------------
class BuildModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructors/Destructor
//------------------------------------------------------------------------------
	public:
		BuildModel(uint number, const QString &url, QObject *parent=nullptr);
		~BuildModel();

	private:
		BuildModel(const BuildModel &) = delete;
		BuildModel & operator=(const BuildModel &) = delete;

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		bool isLoaded() const;
		void http_finished(const QString &content, QNetworkReply::NetworkError errCode, const QString &error);

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void load();

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void loaded();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		bool m_loaded;

//------------------------------------------------------------------------------
// Members declaration
//------------------------------------------------------------------------------
#define BUILD_DECLARE_MEMBER(Type, Member) \
	public: \
		const Type & get##Member() const; \
	public slots: \
		void set##Member(const Type & Member); \
	private: \
		Type m_##Member;
//------------------------------------------------------------------------------
BUILD_DECLARE_MEMBER(uint,			Number)
BUILD_DECLARE_MEMBER(QString,		Url)
BUILD_DECLARE_MEMBER(bool,			Building)
BUILD_DECLARE_MEMBER(QString,		Description)
BUILD_DECLARE_MEMBER(qint64,		Duration)
BUILD_DECLARE_MEMBER(qint64,		EstimatedDuration)
BUILD_DECLARE_MEMBER(QString,		FullDisplayName)
BUILD_DECLARE_MEMBER(QString,		Id)
BUILD_DECLARE_MEMBER(QString,		Result)
BUILD_DECLARE_MEMBER(qint64,		Timestamp)
BUILD_DECLARE_MEMBER(QString,		BuiltOn)
//------------------------------------------------------------------------------
#undef BUILD_DECLARE_MEMBER
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // BUILDMODEL_H
//------------------------------------------------------------------------------
