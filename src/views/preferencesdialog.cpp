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
	m_updateIntervalValidator->setRange(1, 999);
	ui->m_updateIntervalLineEdit->setValidator(m_updateIntervalValidator);

	m_columnsValidator = new QIntValidator(ui->m_columnsLineEdit);
	m_columnsValidator->setRange(1, 100);
	ui->m_columnsLineEdit->setValidator(m_columnsValidator);

	m_jobsMarginValidator = new QIntValidator(ui->m_jobsMarginLineEdit);
	m_jobsMarginValidator->setRange(0, 100);
	ui->m_jobsMarginLineEdit->setValidator(m_jobsMarginValidator);

#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
	ui->m_jenkinsURLLineEdit->setPlaceholderText(tr("Enter url..."));
#endif

	ui->m_jobsNameDescAlignComboBox->addItem(tr("Left"),		(int)(Qt::AlignLeft|Qt::AlignVCenter));
	ui->m_jobsNameDescAlignComboBox->addItem(tr("Center"),	(int)Qt::AlignCenter);
	ui->m_jobsNameDescAlignComboBox->addItem(tr("Right"),		(int)(Qt::AlignRight|Qt::AlignVCenter));
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
	ui->m_jenkinsURLLineEdit->setText(prefs.getJenkinsUrl());

	// Jenkins API Token
	ui->m_APITokenLineEdit->setText(prefs.getAPIToken());

	// Update interval
	ui->m_updateIntervalLineEdit->setText(QString("%1").arg(prefs.getAPIUpdateInterval()));

	// Selected view
	int index = ui->m_viewDisplayComboBox->findData(prefs.getSelectedView());
	ui->m_viewDisplayComboBox->setCurrentIndex((index >= 0)?index:0);

	// Checkboxes
	ui->m_showBuildNumberCheckBox->setChecked(prefs.getShowBuildNumber());
	ui->m_showWeatherCheckBox->setChecked(prefs.getShowWeatherIcon());
	ui->m_showEstEndTimeCheckBox->setChecked(prefs.getShowEstimatedEndTime());
	ui->m_showLastBuildDescCheckBox->setChecked(prefs.getShowLastBuildDescription());
	ui->m_showNodesCheckBox->setChecked(prefs.getShowNodes());
	ui->m_showPositionInQueue->setChecked(prefs.getShowPositionInQueue());
	ui->m_startFullscreenCheckBox->setChecked(prefs.getStartFullscreen());

	// Number of columns
	ui->m_columnsLineEdit->setText(QString("%1").arg(prefs.getColumns()));

	// Jobs margin PX
	ui->m_jobsMarginLineEdit->setText(QString("%1").arg(prefs.getJobsMargin()));

	int comboIndex = ui->m_jobsNameDescAlignComboBox->findData(prefs.getJobsNameDescAlignFlags());
	Q_ASSERT(comboIndex >= 0);
	if(comboIndex >= 0)
		ui->m_jobsNameDescAlignComboBox->setCurrentIndex(comboIndex);
	else
		ui->m_jobsNameDescAlignComboBox->setCurrentIndex(0);
	//----------------------------------------------------------

	ui->m_jenkinsTabWidget->setCurrentIndex(0);

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

	// Premier update, on récupère la view dans les prefs
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
	int pos=0;
	QString text;

	// General tab --------------------------------------------------------------
	Prefs.setStartFullscreen(ui->m_startFullscreenCheckBox->isChecked());

	// Jenkins tab --------------------------------------------------------------
	text = ui->m_updateIntervalLineEdit->text();

	if(m_updateIntervalValidator->validate(text, pos) == QValidator::Acceptable)
		Prefs.setAPIUpdateInterval(ui->m_updateIntervalLineEdit->text().toUInt());
	else{
		ui->m_updateIntervalLineEdit->setText(QString("%1").arg(Prefs.getAPIUpdateInterval()));
	}

	Prefs.setAPIUpdateInterval(ui->m_updateIntervalLineEdit->text().toUInt());
	Prefs.setJenkinsUrl(ui->m_jenkinsURLLineEdit->text());
	Prefs.setAPIToken(ui->m_APITokenLineEdit->text());
	QVariant selectedViewData = ui->m_viewDisplayComboBox->itemData(ui->m_viewDisplayComboBox->currentIndex());
	Prefs.setSelectedView((selectedViewData.isValid())?selectedViewData.toString():"");

	// Display tab --------------------------------------------------------------
	text = ui->m_columnsLineEdit->text();
	if(m_columnsValidator->validate(text, pos) == QValidator::Acceptable)
		Prefs.setColumns(ui->m_columnsLineEdit->text().toUInt());
	else
		ui->m_columnsLineEdit->setText(QString("%1").arg(Prefs.getColumns()));

	text = ui->m_jobsMarginLineEdit->text();
	if(m_jobsMarginValidator->validate(text, pos) == QValidator::Acceptable)
		Prefs.setJobsMargin(ui->m_jobsMarginLineEdit->text().toUInt());
	else
		ui->m_jobsMarginLineEdit->setText(QString("%1").arg(Prefs.getJobsMargin()));

	Prefs.setShowBuildNumber(ui->m_showBuildNumberCheckBox->isChecked());
	Prefs.setShowWeatherIcon(ui->m_showWeatherCheckBox->isChecked());
	Prefs.setShowEstimatedEndTime(ui->m_showEstEndTimeCheckBox->isChecked());
	Prefs.setShowLastBuildDescription(ui->m_showLastBuildDescCheckBox->isChecked());
	Prefs.setShowNodes(ui->m_showNodesCheckBox->isChecked());
	Prefs.setShowPositionInQueue(ui->m_showPositionInQueue->isChecked());

	const QComboBox *cb = ui->m_jobsNameDescAlignComboBox;
	Prefs.setJobsNameDescAlignFlags(cb->itemData(cb->currentIndex()).toInt());
}
//------------------------------------------------------------------------------
