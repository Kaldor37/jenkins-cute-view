//------------------------------------------------------------------------------
#include "views/preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "preferences.h"

#include <limits.h>
#include <QIntValidator>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QDebug>
//------------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog){
	ui->setupUi(this);

	m_updateIntervalValidator = new QIntValidator(ui->m_updateIntervalLineEdit);
	m_updateIntervalValidator->setRange(1, UINT_MAX);
	ui->m_updateIntervalLineEdit->setValidator(m_updateIntervalValidator);
}
//------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog(){
	delete ui;
}
//------------------------------------------------------------------------------
void PreferencesDialog::showEvent(QShowEvent * event){

	// Load prefs ----------------------------------------------
	const Preferences &prefs = Prefs;

	// Jenkins URL
	QString jenkinsUrl = prefs.getJenkinsUrl();
	if(!jenkinsUrl.isEmpty())
		ui->m_jenkinsURLLineEdit->setText(jenkinsUrl);

	// Update interval
	ui->m_updateIntervalLineEdit->setText(QString("%1").arg(prefs.getAPIUpdateInterval()));

	// Selected view
	int index = ui->m_viewDisplayComboBox->findData(prefs.getSelectedView());
	ui->m_viewDisplayComboBox->setCurrentIndex((index >= 0)?index:0);
	//----------------------------------------------------------

	QWidget::showEvent(event);
}
//------------------------------------------------------------------------------
void PreferencesDialog::viewsList_updated(const QStringList &viewsList, const QString &primaryView){
	bool firstUpdate = (ui->m_viewDisplayComboBox->count() == 0);

	// Update primary text
	int primaryIndex = ui->m_viewDisplayComboBox->findData(QVariant());
	QString primaryText = tr("Default (%1)").arg(primaryView);
	if(primaryIndex >= 0)
		ui->m_viewDisplayComboBox->setItemText(primaryIndex, primaryText);
	else
		ui->m_viewDisplayComboBox->addItem(primaryText, QVariant());

	// Update other texts
	foreach(const QString &view, viewsList){
		// Skip primary view
		if(view == primaryView)
			continue;

		QVariant viewData(view);
		int itemIndex = ui->m_viewDisplayComboBox->findData(viewData);
		if(itemIndex >= 0)
			ui->m_viewDisplayComboBox->setItemText(itemIndex, view);
		else
			ui->m_viewDisplayComboBox->addItem(view, viewData);
	}

	// Premier update, on r�cup�re la view dans les prefs
	if(firstUpdate){
		int index = ui->m_viewDisplayComboBox->findData(Prefs.getSelectedView());
		if(index >= 0)
			ui->m_viewDisplayComboBox->setCurrentIndex(index);
	}
}
//------------------------------------------------------------------------------
void PreferencesDialog::on_m_buttonBox_clicked(QAbstractButton *btn){
	switch(ui->m_buttonBox->buttonRole(btn)){
		case QDialogButtonBox::AcceptRole:
		case QDialogButtonBox::YesRole:
			savePreferences();
			accept();
		break;

		case QDialogButtonBox::ApplyRole:
			savePreferences();
		break;

		case QDialogButtonBox::RejectRole:
		case QDialogButtonBox::NoRole:
			reject();
		break;

		default:
			qWarning()<<"PreferencesDialog::on_m_OKCancelButtonBox_clicked() - Invalid role for button "<<btn->text();
		break;
	}
}
//------------------------------------------------------------------------------
void PreferencesDialog::savePreferences(){
	// Jenkins tab
	Prefs.setAPIUpdateInterval(ui->m_updateIntervalLineEdit->text().toUInt());
	Prefs.setJenkinsUrl(ui->m_jenkinsURLLineEdit->text());

	// Display tab
	QVariant selectedViewData = ui->m_viewDisplayComboBox->itemData(ui->m_viewDisplayComboBox->currentIndex());
	Prefs.setSelectedView((selectedViewData.isValid())?selectedViewData.toString():"");
}
//------------------------------------------------------------------------------
