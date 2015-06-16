//------------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//------------------------------------------------------------------------------
#include <QMainWindow>
//------------------------------------------------------------------------------
class PreferencesDialog;
class AboutDialog;
class QShortcut;
class JenkinsGraphicsView;
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
		explicit MainWindow(QWidget *parent = nullptr);
		~MainWindow();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void showPrefsDialog();
		void showAboutDialog();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		JenkinsGraphicsView * getGraphicsView();

//------------------------------------------------------------------------------
// Signals
//------------------------------------------------------------------------------
	signals:
		void preferencesUpdated();
		void viewsNamesUpdated(QStringList,QString);
		void fullscreenModeChanged(bool enabled);

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
		void graphicsView_contextMenuShown();
		void graphicsView_contextMenuHidden();

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

		// Empty cursor
		QCursor				*m_emptyCursor;
};
//------------------------------------------------------------------------------
#endif // MAINWINDOW_H
//------------------------------------------------------------------------------
