//------------------------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencesdialog.h"
#include "application.h"

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
	m_prefsDialog(0)
{
	ui->setupUi(this);

	m_fullscreenShortcut = new QShortcut(QKeySequence(Qt::Key_F11), this);
	m_fullscreenShortcut->setContext(Qt::WindowShortcut);
	connect(m_fullscreenShortcut, SIGNAL(activated()), SLOT(fullScreenShortcut_activated()));

	// Moves this window to the center of the screen
	const QDesktopWidget &dw = *QApplication::desktop();
	QRect rect = frameGeometry();
	rect.moveCenter(dw.availableGeometry().center());
	move(rect.topLeft());

	createPrefsDialog();

	// Display window
	show();
}
//------------------------------------------------------------------------------
MainWindow::~MainWindow(){
	delete ui;
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
	}
	// Set to normal
	else{
		showNormal();
		ui->m_menuBar->setVisible(true);
	}
}
//------------------------------------------------------------------------------
void MainWindow::on_m_actionAbout_triggered(){
	qDebug()<<"MainWindow::on_m_actionAbout_triggered - TODO";
}
//------------------------------------------------------------------------------
void MainWindow::fullScreenShortcut_activated(){
	ui->m_actionFullscreen->trigger();
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
