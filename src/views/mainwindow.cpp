//------------------------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"
#include "application.h"
#include "preferences.h"

#include <QDebug>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QRect>
#include <QShortcut>
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_prefsDialog(0),
	m_aboutDialog(0),
	m_fullscreenShortcut(0),
	m_emptyCursor(0)
{
	ui->setupUi(this);

	m_fullscreenShortcut = new QShortcut(QKeySequence(Qt::Key_F11), this);
	m_fullscreenShortcut->setContext(Qt::WindowShortcut);
	connect(m_fullscreenShortcut, SIGNAL(activated()), SLOT(fullScreenShortcut_activated()));

	QPixmap emptyCursorPixmap(QSize(1,1));
	emptyCursorPixmap.fill(Qt::transparent);
	m_emptyCursor = new QCursor(emptyCursorPixmap);

	// Moves this window to the center of the screen
	const QDesktopWidget &dw = *QApplication::desktop();
	QRect rect = frameGeometry();
	rect.moveCenter(dw.availableGeometry().center());
	move(rect.topLeft());

	createPrefsDialog();

	connect(ui->m_graphicsView, SIGNAL(fullScreenTriggered()), SLOT(fullScreenShortcut_activated()));
	connect(ui->m_graphicsView, SIGNAL(preferencesTriggered()), SLOT(on_m_actionPreferences_triggered()));
	connect(ui->m_graphicsView, SIGNAL(quitTriggered()), SLOT(on_m_actionQuit_triggered()));
	connect(this, SIGNAL(fullscreenModeChanged(bool)), ui->m_graphicsView, SLOT(fullscreenModeChanged(bool)));

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
		QObject::connect(this, SIGNAL(viewsNamesUpdated(QStringList,QString)), m_prefsDialog, SLOT(viewsList_updated(QStringList,QString)));
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
