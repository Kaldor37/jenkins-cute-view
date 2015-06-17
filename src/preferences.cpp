//------------------------------------------------------------------------------
#include "preferences.h"
#include <QFontDatabase>
//------------------------------------------------------------------------------
// Static members initialization
//------------------------------------------------------------------------------
Preferences * Preferences::m_instance = nullptr;
//------------------------------------------------------------------------------
bool validateFontFamily(const QString &familyName){
	return QFontDatabase().families(QFontDatabase::Latin).contains(familyName);
}
//------------------------------------------------------------------------------
#define PREFS_IMPL_FIELD(Type, FieldName, VariantConvert, PrefFieldName, DefaultValue) \
	const char * Preferences::FIELD_NAME_##FieldName = PrefFieldName; \
	const Type Preferences::DEFAULT_##FieldName = DefaultValue; \
	Type Preferences::get##FieldName() const { \
		return value(FIELD_NAME_##FieldName, DEFAULT_##FieldName).to##VariantConvert(); \
	} \
	void Preferences::set##FieldName(const Type & value){ \
		if(get##FieldName() != value){ \
			setValue(FIELD_NAME_##FieldName, value); \
			emit sig##FieldName##Changed(value); \
		} \
	}
#define PREFS_IMPL_STRING_FIELD(FieldName, PrefFieldName, DefaultValue)		PREFS_IMPL_FIELD(QString, FieldName, String, PrefFieldName, DefaultValue)
#define PREFS_IMPL_INT_FIELD(FieldName, PrefFieldName, DefaultValue)			PREFS_IMPL_FIELD(int, FieldName, Int, PrefFieldName, DefaultValue)
#define PREFS_IMPL_UINT_FIELD(FieldName, PrefFieldName, DefaultValue)		PREFS_IMPL_FIELD(uint, FieldName, UInt, PrefFieldName, DefaultValue)
#define PREFS_IMPL_BOOL_FIELD(FieldName, PrefFieldName, DefaultValue)		PREFS_IMPL_FIELD(bool, FieldName, Bool, PrefFieldName, DefaultValue)
//------------------------------------------------------------------------------
PREFS_IMPL_STRING_FIELD(JenkinsUrl,					"Jenkins/url", "")
PREFS_IMPL_UINT_FIELD(APIUpdateInterval,			"Jenkins/APIUpdateInterval", 15)
PREFS_IMPL_STRING_FIELD(APIUserID,					"Jenkins/APIUserID", "")
PREFS_IMPL_STRING_FIELD(APIToken,					"Jenkins/APIToken", "")
PREFS_IMPL_STRING_FIELD(SelectedView,				"Display/SelectedView", "")
PREFS_IMPL_BOOL_FIELD(ShowBuildNumber,				"Display/ShowBuildNumber", true)
PREFS_IMPL_BOOL_FIELD(ShowWeatherIcon,				"Display/ShowWeatherIcon", true)
PREFS_IMPL_STRING_FIELD(WeatherIconsTheme,		"Display/WeatherIconsTheme", "weather-icons-theme0")
PREFS_IMPL_BOOL_FIELD(ShowEstimatedEndTime,		"Display/ShowEstimatedEndTime", true)
PREFS_IMPL_BOOL_FIELD(ShowLastBuildDescription,	"Display/ShowLastBuildDescription", true)
PREFS_IMPL_UINT_FIELD(Columns,						"Display/Columns", 1)
PREFS_IMPL_BOOL_FIELD(ShowNodes,						"Display/ShowNodes", true)
PREFS_IMPL_BOOL_FIELD(ShowPositionInQueue,		"Display/ShowPositionInQueue", true)
PREFS_IMPL_UINT_FIELD(JobsMargin,					"Display/JobsMargin", 2)
PREFS_IMPL_INT_FIELD(JobsNameDescAlignFlags,		"Display/JobsNameDescAlignFlags", Qt::AlignCenter)
PREFS_IMPL_STRING_FIELD(Font,							"Display/Font", "Arial")
PREFS_IMPL_BOOL_FIELD(StartFullscreen,				"General/StartFullscreen", false)
//------------------------------------------------------------------------------
#undef PREFS_IMPL_UINT_FIELD
#undef PREFS_IMPL_STRING_FIELD
#undef PREFS_IMPL_FIELD
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
Preferences::Preferences(QObject *parent) : QSettings(parent){
	if(!validateFontFamily(getFont())){
		setFont(DEFAULT_Font);
	}
}
//------------------------------------------------------------------------------
Preferences::~Preferences(){
	sync();
}
//------------------------------------------------------------------------------
// Singleton static functions
//------------------------------------------------------------------------------
void Preferences::create(QObject *parent){
	Q_ASSERT(!m_instance);
	m_instance = new Preferences(parent);
}
//------------------------------------------------------------------------------
void Preferences::destroy(){
	Q_ASSERT(m_instance);
	delete m_instance;
	m_instance = nullptr;
}
//------------------------------------------------------------------------------
Preferences & Preferences::instance(){
	Q_ASSERT(m_instance);
	return *m_instance;
}
//------------------------------------------------------------------------------
