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
		static void create(QObject *parent=0);
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
		explicit Preferences(QObject *parent=0);
		Preferences(const Preferences&);
		Preferences& operator=(const Preferences&);

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
//------------------------------------------------------------------------------
#undef PREFS_DECLARE_FIELD

//------------------------------------------------------------------------------
// Signals (cannot be declared with macro)
//------------------------------------------------------------------------------
	signals:
		void sigJenkinsUrlChanged(QString);
		void sigAPIUpdateIntervalChanged(uint);
		void sigSelectedViewChanged(QString);
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // PREFERENCES_H
//------------------------------------------------------------------------------
