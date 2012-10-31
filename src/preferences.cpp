//------------------------------------------------------------------------------
#include "preferences.h"
//------------------------------------------------------------------------------
// Static members initialization
//------------------------------------------------------------------------------
Preferences * Preferences::m_instance = 0;
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
#define PREFS_IMPL_UINT_FIELD(FieldName, PrefFieldName, DefaultValue)		PREFS_IMPL_FIELD(uint, FieldName, UInt, PrefFieldName, DefaultValue)
//------------------------------------------------------------------------------
PREFS_IMPL_STRING_FIELD(JenkinsUrl, "Jenkins/url", "")
PREFS_IMPL_STRING_FIELD(SelectedView, "Display/SelectedView", "")
PREFS_IMPL_UINT_FIELD(APIUpdateInterval, "Jenkins/APIUpdateInterval", 15)
//------------------------------------------------------------------------------
#undef PREFS_IMPL_UINT_FIELD
#undef PREFS_IMPL_STRING_FIELD
#undef PREFS_IMPL_FIELD
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
Preferences::Preferences(QObject *parent/*=0*/) : QSettings(parent){
}
//------------------------------------------------------------------------------
Preferences::~Preferences(){
	sync();
}
//------------------------------------------------------------------------------
// Singleton static functions
//------------------------------------------------------------------------------
void Preferences::create(QObject *parent/*=0*/){
	Q_ASSERT(m_instance == 0);
	m_instance = new Preferences(parent);
}
//------------------------------------------------------------------------------
void Preferences::destroy(){
	Q_ASSERT(m_instance != 0);
	delete m_instance;
	m_instance = 0;
}
//------------------------------------------------------------------------------
Preferences & Preferences::instance(){
	Q_ASSERT(m_instance != 0);
	return *m_instance;
}
//------------------------------------------------------------------------------
