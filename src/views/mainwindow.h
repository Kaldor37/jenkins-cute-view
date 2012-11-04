//------------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//------------------------------------------------------------------------------
#include <QMainWindow>
//------------------------------------------------------------------------------
class PreferencesDialog;
class AboutDialog;
class QShortcut;
//------------------------------------------------------------------------------
namespace Ui {
	class MainWindow;
}
//------------------------------------------------------------------------------
class MainWindow : public QMainWindow{
		Q_OBJECT
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void showPrefsDialog();
		void showAboutDialog();

//------------------------------------------------------------------------------
// Protected functions
//------------------------------------------------------------------------------
	signals:
		void preferencesUpdated();
		void viewsNamesUpdated(QStringList,QString);

//------------------------------------------------------------------------------
// Protected functions
//------------------------------------------------------------------------------
	protected:

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void on_m_actionQuit_triggered();
		void on_m_actionPreferences_triggered();
		void on_m_actionFullscreen_triggered();
		void on_m_actionAbout_triggered();

		void fullScreenShortcut_activated();

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void createPrefsDialog();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		Ui::MainWindow *ui;

		// Dialogs
		PreferencesDialog	*m_prefsDialog;
		AboutDialog			*m_aboutDialog;

		// Shortcuts
		QShortcut			*m_fullscreenShortcut;
};
//------------------------------------------------------------------------------
#endif // MAINWINDOW_H
//------------------------------------------------------------------------------
