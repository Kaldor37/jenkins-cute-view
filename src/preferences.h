//------------------------------------------------------------------------------
#ifndef PREFERENCES_H
#define PREFERENCES_H
//------------------------------------------------------------------------------
#include <QSettings>
//------------------------------------------------------------------------------
// Easy access to Preferences singleton
#define Prefs	Preferences::instance()
//------------------------------------------------------------------------------
class Preferences : public QSettings {
	Q_OBJECT
//------------------------------------------------------------------------------
// Singleton public static functions
//------------------------------------------------------------------------------
	public:
		static void create(QObject *parent=nullptr);
		static void destroy();
		static Preferences & instance();

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
	public:
		~Preferences();

//------------------------------------------------------------------------------
// Constructor/Copy constructor/operator=
//------------------------------------------------------------------------------
	private:
		explicit Preferences(QObject *parent=nullptr);

		Preferences(const Preferences&) = delete;
		Preferences& operator=(const Preferences&) = delete;

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		static Preferences * m_instance;

//------------------------------------------------------------------------------
// Fields declaration
//------------------------------------------------------------------------------
/**
 * Preference field declaration :
 * - A getter
 * - A setter slot
 * - A private constant field name
 * - A private constant default value
 */
#define PREFS_DECLARE_FIELD(Type, FieldName) \
	public: \
		Type get##FieldName() const; \
	public slots: \
		void set##FieldName(const Type &); \
	public: \
		static const char *	FIELD_NAME_##FieldName; \
		static const Type		DEFAULT_##FieldName;
//------------------------------------------------------------------------------
PREFS_DECLARE_FIELD(QString,	JenkinsUrl)
PREFS_DECLARE_FIELD(uint,		APIUpdateInterval)
PREFS_DECLARE_FIELD(QString,	SelectedView)
PREFS_DECLARE_FIELD(bool,		ShowBuildNumber)
PREFS_DECLARE_FIELD(bool,		ShowWeatherIcon)
PREFS_DECLARE_FIELD(QString,	WeatherIconsTheme)
PREFS_DECLARE_FIELD(bool,		ShowEstimatedEndTime)
PREFS_DECLARE_FIELD(bool,		ShowLastBuildDescription)
PREFS_DECLARE_FIELD(uint,		Columns)
PREFS_DECLARE_FIELD(bool,		ShowNodes)
PREFS_DECLARE_FIELD(bool,		ShowNodesResponseTime)
PREFS_DECLARE_FIELD(bool,		ShowPositionInQueue)
PREFS_DECLARE_FIELD(uint,		JobsMargin)
PREFS_DECLARE_FIELD(int,		JobsNameDescAlignFlags)
PREFS_DECLARE_FIELD(QString,	APIUserID)
PREFS_DECLARE_FIELD(QString,	APIToken)
PREFS_DECLARE_FIELD(bool,		StartFullscreen)
PREFS_DECLARE_FIELD(QString,	Font)
//------------------------------------------------------------------------------
#undef PREFS_DECLARE_FIELD

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void sigJenkinsUrlChanged(QString);
		void sigAPIUpdateIntervalChanged(uint);
		void sigSelectedViewChanged(QString);
		void sigShowBuildNumberChanged(bool);
		void sigShowWeatherIconChanged(bool);
		void sigWeatherIconsThemeChanged(QString);
		void sigShowEstimatedEndTimeChanged(bool);
		void sigShowLastBuildDescriptionChanged(bool);
		void sigColumnsChanged(uint);
		void sigShowNodesChanged(bool);
		void sigShowNodesResponseTimeChanged(bool);
		void sigShowPositionInQueueChanged(bool);
		void sigJobsMarginChanged(uint);
		void sigJobsNameDescAlignFlagsChanged(int);
		void sigAPIUserIDChanged(QString);
		void sigAPITokenChanged(QString);
		void sigStartFullscreenChanged(bool);
		void sigFontChanged(QString);

//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // PREFERENCES_H
//------------------------------------------------------------------------------
