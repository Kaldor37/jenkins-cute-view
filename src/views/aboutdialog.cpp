#include "views/aboutdialog.h"
#include "ui_aboutdialog.h"
#include "application.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog){
	ui->setupUi(this);

	// Version label
	ui->m_versionLabel->setText(ui->m_versionLabel->text().arg(App.applicationVersion()));

#ifdef GIT_REVISION
	ui->m_versionLabel->setToolTip(ui->m_versionLabel->toolTip().arg(GIT_REVISION));
#else
	ui->m_versionLabel->setToolTip(QString());
#endif

	// Qt version label
	ui->m_qtVersionLabel->setText(ui->m_qtVersionLabel->text().arg(QT_VERSION_STR));

	this->adjustSize();
}

AboutDialog::~AboutDialog(){
	delete ui;
}
