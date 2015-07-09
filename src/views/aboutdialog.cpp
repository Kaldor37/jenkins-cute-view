#include "views/aboutdialog.h"
#include "ui_aboutdialog.h"
#include "application.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog){
	ui->setupUi(this);

	// Version label
	ui->m_versionLabel->setText(ui->m_versionLabel->text().arg(App.applicationVersion().left(7)));
	ui->m_versionLabel->setToolTip(App.applicationVersion());

	// Qt version label
	ui->m_qtVersionLabel->setText(ui->m_qtVersionLabel->text().arg(QT_VERSION_STR));

	this->adjustSize();
}

AboutDialog::~AboutDialog(){
	delete ui;
}
