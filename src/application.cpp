//------------------------------------------------------------------------------
#include "application.h"
#include <QDebug>
//------------------------------------------------------------------------------
// Constants initialization
//------------------------------------------------------------------------------
const QString Application::appName("JenkinsCuteView");
const QString Application::appVersion("0.9.55");
const QString Application::orgName("JenkinsCuteView");
//------------------------------------------------------------------------------
// Static members initialization
//------------------------------------------------------------------------------
Application * Application::m_instance = nullptr;
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
Application::Application(int& argc, char* argv[]) : QApplication(argc, argv){
	setApplicationName(appName);
	setApplicationVersion(appVersion);
	setOrganizationName(orgName);

	m_verbose = false;
	for(int i=0 ; i < argc ; ++i){
		QString arg(argv[i]);
		if(arg == "-v" || arg == "--verbose"){
			qDebug()<<"Verbose mode enabled";
			m_verbose = true;
		}
	}
}
//------------------------------------------------------------------------------
Application::~Application(){
}
//------------------------------------------------------------------------------
// Singleton static functions
//------------------------------------------------------------------------------
void Application::create(int& argc, char* argv[]){
	Q_ASSERT(!m_instance);
	m_instance = new Application(argc, argv);
}
//------------------------------------------------------------------------------
void Application::destroy(){
	Q_ASSERT(m_instance);
	delete m_instance;
	m_instance = nullptr;
}
//------------------------------------------------------------------------------
Application & Application::instance(){
	Q_ASSERT(m_instance);
	return *m_instance;
}
//------------------------------------------------------------------------------
bool Application::verbose() const{
	return m_verbose;
}
//------------------------------------------------------------------------------
