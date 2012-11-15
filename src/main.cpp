//------------------------------------------------------------------------------
#include "application.h"
#include "preferences.h"
#include "utils/httpgetter.h"
#include "views/mainwindow.h"
#include "controllers/jenkinscontroller.h"

#include <QDebug>
#include <QResource>
//------------------------------------------------------------------------------
int main(int argc, char *argv[]){
	int retCode = 0;

	Application::create(argc, argv);
	Preferences::create();
	HttpGetter::create();

	/* For now, weather icons are embeded
	bool weatherIconsLoaded = QResource::registerResource("weather-icons.rcc");
	Q_ASSERT(weatherIconsLoaded);
	if(!weatherIconsLoaded)
		qWarning()<<"Cannot load weather icons (weather-icons.rcc)";
	*/

	// Application scope
	{
		// Init controllers
		JenkinsController jc;

		// Init and show main window
		MainWindow w;
		jc.control(&w);

		jc.start();

		// Star app
		try{
			retCode = App.exec();
		}
		catch(int ex){
			qWarning()<<"Application closed unexpectedly : "<<ex;
		}
	}

	HttpGetter::destroy();
	Preferences::destroy();
	Application::destroy();

	return retCode;
}
