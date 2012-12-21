//------------------------------------------------------------------------------
#ifndef APPLICATION_H
#define APPLICATION_H
//------------------------------------------------------------------------------
#include <QApplication>
//------------------------------------------------------------------------------
// Easy access to Application singleton
#define App	Application::instance()
//------------------------------------------------------------------------------
class Application : public QApplication{
	Q_OBJECT
//------------------------------------------------------------------------------
// Singleton public static functions
//------------------------------------------------------------------------------
	public:
		static void create(int& argc, char* argv[]);
		static void destroy();
		static Application & instance();

		// Debug
	public slots:
		void qObjectDestroyed();

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
	public:
		~Application();

//------------------------------------------------------------------------------
// Constructor/Copy constructor/operator=
//------------------------------------------------------------------------------
	private:
		explicit Application(int& argc, char* argv[]);
		Application(const Application&);
		Application& operator=(const Application&);

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		static Application * m_instance;

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
		static const QString appName;
		static const QString appVersion;
		static const QString orgName;
};
//------------------------------------------------------------------------------
#endif // APPLICATION_H
//------------------------------------------------------------------------------
