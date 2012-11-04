#include "views/aboutdialog.h"
#include "ui_aboutdialog.h"
#include "application.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog){
	ui->setupUi(this);
	ui->m_versionLabel->setText(ui->m_versionLabel->text().arg(App.applicationVersion()));
}

AboutDialog::~AboutDialog(){
	delete ui;
}
