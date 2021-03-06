//------------------------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"
#include "application.h"
#include "preferences.h"

#include <QKeyEvent>
#include <QDesktopWidget>
#include <QRect>
#include <QShortcut>
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_fullscreenShortcut = new QShortcut(QKeySequence(Qt::Key_F11), this);
	m_fullscreenShortcut->setContext(Qt::WindowShortcut);
	QObject::connect(m_fullscreenShortcut, &QShortcut::activated, this, &MainWindow::fullScreenShortcut_activated);

	QPixmap emptyCursorPixmap(QSize(1,1));
	emptyCursorPixmap.fill(Qt::transparent);
	m_emptyCursor = new QCursor(emptyCursorPixmap);

	// Moves this window to the center of the screen
	const QDesktopWidget &dw = *QApplication::desktop();
	QRect rect = frameGeometry();
	rect.moveCenter(dw.availableGeometry().center());
	move(rect.topLeft());

	createPrefsDialog();

	QObject::connect(ui->m_graphicsView, &JenkinsGraphicsView::fullScreenTriggered, this, &MainWindow::fullScreenShortcut_activated);
	QObject::connect(ui->m_graphicsView, &JenkinsGraphicsView::preferencesTriggered, this, &MainWindow::on_m_actionPreferences_triggered);
	QObject::connect(ui->m_graphicsView, &JenkinsGraphicsView::quitTriggered, this, &MainWindow::on_m_actionQuit_triggered);
	QObject::connect(this, &MainWindow::fullscreenModeChanged, ui->m_graphicsView, &JenkinsGraphicsView::fullscreenModeChanged);

	// Display window
	show();

	// Start in fullscreen mode
	if(Prefs.getStartFullscreen())
		ui->m_actionFullscreen->trigger();
}
//------------------------------------------------------------------------------
MainWindow::~MainWindow(){
	delete ui;

	if(m_emptyCursor)
		delete m_emptyCursor;
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void MainWindow::createPrefsDialog(){
	if(!m_prefsDialog){
		m_prefsDialog = new PreferencesDialog(this);
		QObject::connect(this, &MainWindow::viewsNamesUpdated, m_prefsDialog, &PreferencesDialog::viewsList_updated);
	}
}
//------------------------------------------------------------------------------
void MainWindow::showPrefsDialog(){
	Q_ASSERT(m_prefsDialog);
	if(!m_prefsDialog->isVisible())
		m_prefsDialog->show();
}
//------------------------------------------------------------------------------
void MainWindow::showAboutDialog(){
	if(!m_aboutDialog)
		m_aboutDialog = new AboutDialog(this);

	if(!m_aboutDialog->isVisible())
		m_aboutDialog->show();
}
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
JenkinsGraphicsView * MainWindow::getGraphicsView(){
	return ui->m_graphicsView;
}
//------------------------------------------------------------------------------
// Protected functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
void MainWindow::on_m_actionQuit_triggered(){
	close();
}
//------------------------------------------------------------------------------
void MainWindow::on_m_actionPreferences_triggered(){
	showPrefsDialog();
}
//------------------------------------------------------------------------------
void MainWindow::on_m_actionFullscreen_triggered(){
	// Set fullscreen
	if(!isFullScreen()){
		showFullScreen();
		ui->m_menuBar->setVisible(false);
		setCursor(*m_emptyCursor);
		emit fullscreenModeChanged(true);
	}
	// Set to normal
	else{
		setCursor(Qt::ArrowCursor);
		showNormal();
		ui->m_menuBar->setVisible(true);
		emit fullscreenModeChanged(false);
	}
}
//------------------------------------------------------------------------------
void MainWindow::on_m_actionAbout_triggered(){
	showAboutDialog();
}
//------------------------------------------------------------------------------
void MainWindow::fullScreenShortcut_activated(){
	ui->m_actionFullscreen->trigger();
}
//------------------------------------------------------------------------------
void MainWindow::graphicsView_contextMenuShown(){
	setCursor(Qt::ArrowCursor);
}
//------------------------------------------------------------------------------
void MainWindow::graphicsView_contextMenuHidden(){
	if(isFullScreen())
		setCursor(*m_emptyCursor);
}
//------------------------------------------------------------------------------
