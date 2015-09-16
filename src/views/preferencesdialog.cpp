//------------------------------------------------------------------------------
#include "views/preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "preferences.h"
#include "utils/httpgetter.h"

#include <limits.h>
#include <QIntValidator>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QFontDatabase>
//------------------------------------------------------------------------------
PreferencesDialog::PreferencesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog){
	ui->setupUi(this);

	ui->m_jenkinsURLLineEdit->setPlaceholderText(tr("Enter url..."));

	ui->m_jobsNameDescAlignComboBox->addItem(tr("Left"),		(int)(Qt::AlignLeft|Qt::AlignVCenter));
	ui->m_jobsNameDescAlignComboBox->addItem(tr("Center"),	(int)Qt::AlignCenter);
	ui->m_jobsNameDescAlignComboBox->addItem(tr("Right"),		(int)(Qt::AlignRight|Qt::AlignVCenter));

	ui->m_weatherIconsThemeComboBox->addItem(tr("Meteocons"),	"weather-icons-theme0");
	ui->m_weatherIconsThemeComboBox->addItem(tr("Garmahis"),		"weather-icons-theme1");
	ui->m_weatherIconsThemeComboBox->addItem(tr("Dark"),			"weather-icons-theme2");

	const QStringList fontFamilies = QFontDatabase().families(QFontDatabase::Latin);
	for(const QString &font : fontFamilies){
		ui->m_fontComboBox->addItem(font, font);
	}
}
//------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog(){
	delete ui;
}
//------------------------------------------------------------------------------
void PreferencesDialog::showEvent(QShowEvent * event){

	// Load prefs ----------------------------------------------
	const Preferences &prefs = Prefs;
	int comboIndex = 0;

	// Jenkins URL
	ui->m_jenkinsURLLineEdit->setText(prefs.getJenkinsUrl());

	// Jenkins API UserID
	ui->m_APIUserIdLineEdit->setText(prefs.getAPIUserID());

	// Jenkins API Token
	ui->m_APITokenLineEdit->setText(prefs.getAPIToken());

	// Update interval
	ui->m_updateIntervalSpinBox->setValue(prefs.getAPIUpdateInterval());

	// Selected view
	int index = ui->m_viewDisplayComboBox->findData(prefs.getSelectedView());
	ui->m_viewDisplayComboBox->setCurrentIndex((index >= 0)?index:0);

	// Checkboxes
	ui->m_showBuildNumberCheckBox->setChecked(prefs.getShowBuildNumber());
	ui->m_showWeatherCheckBox->setChecked(prefs.getShowWeatherIcon());
	ui->m_showEstEndTimeCheckBox->setChecked(prefs.getShowEstimatedEndTime());
	ui->m_showLastBuildDescCheckBox->setChecked(prefs.getShowLastBuildDescription());
	ui->m_showNodesCheckBox->setChecked(prefs.getShowNodes());
	ui->m_showNodesResponseTimeCheckBox->setChecked(prefs.getShowNodesResponseTime());
	ui->m_showPositionInQueue->setChecked(prefs.getShowPositionInQueue());
	ui->m_startFullscreenCheckBox->setChecked(prefs.getStartFullscreen());

	// Number of columns
	ui->m_columnsSpinBox->setValue(prefs.getColumns());

	// Jobs margin PX
	ui->m_jobsMarginSpinBox->setValue(prefs.getJobsMargin());

	comboIndex = ui->m_jobsNameDescAlignComboBox->findData(prefs.getJobsNameDescAlignFlags());
	ui->m_jobsNameDescAlignComboBox->setCurrentIndex((comboIndex >= 0)?comboIndex:0);

	comboIndex = ui->m_weatherIconsThemeComboBox->findData(prefs.getWeatherIconsTheme());
	ui->m_weatherIconsThemeComboBox->setCurrentIndex((comboIndex >= 0)?comboIndex:0);

	comboIndex = ui->m_fontComboBox->findData(prefs.getFont());
	ui->m_fontComboBox->setCurrentIndex((comboIndex >= 0)?comboIndex:0);

	ui->m_runningJobsRedrawTimeSpinBox->setValue(prefs.getRunningJobsRedrawTime());
	//----------------------------------------------------------

	ui->m_jenkinsTabWidget->setCurrentIndex(0);

	QWidget::showEvent(event);
}
//------------------------------------------------------------------------------
void PreferencesDialog::viewsList_updated(const QStringList &viewsList, const QString &primaryView){
	Q_ASSERT(viewsList.size() > 0);
	Q_UNUSED(primaryView);

	const bool firstUpdate = (ui->m_viewDisplayComboBox->count() == 0);

	for(QString view : viewsList){
		const QVariant viewData(view);
		const int itemIndex = ui->m_viewDisplayComboBox->findData(viewData);
		if(itemIndex == -1){
			ui->m_viewDisplayComboBox->addItem(view, viewData);
		}
	}

	for(int i=0 ; i < ui->m_viewDisplayComboBox->count() ; ){
		if(!viewsList.contains(ui->m_viewDisplayComboBox->itemText(i)))
			ui->m_viewDisplayComboBox->removeItem(i);
		else
			++i;
	}

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
	// General tab --------------------------------------------------------------
	Prefs.setStartFullscreen(ui->m_startFullscreenCheckBox->isChecked());

	// Jenkins tab --------------------------------------------------------------
	Prefs.setAPIUpdateInterval(ui->m_updateIntervalSpinBox->value());
	Prefs.setJenkinsUrl(ui->m_jenkinsURLLineEdit->text());
	Prefs.setAPIUserID(ui->m_APIUserIdLineEdit->text());
	Prefs.setAPIToken(ui->m_APITokenLineEdit->text());
	HttpGetter::instance().setBasicAuthorization(Prefs.getAPIUserID(), Prefs.getAPIToken());

	QVariant selectedViewData = ui->m_viewDisplayComboBox->itemData(ui->m_viewDisplayComboBox->currentIndex());
	Prefs.setSelectedView((selectedViewData.isValid())?selectedViewData.toString():"");

	// Display tab --------------------------------------------------------------
	Prefs.setColumns(ui->m_columnsSpinBox->value());
	Prefs.setJobsMargin(ui->m_jobsMarginSpinBox->value());

	Prefs.setShowBuildNumber(ui->m_showBuildNumberCheckBox->isChecked());
	Prefs.setShowWeatherIcon(ui->m_showWeatherCheckBox->isChecked());
	Prefs.setShowEstimatedEndTime(ui->m_showEstEndTimeCheckBox->isChecked());
	Prefs.setShowLastBuildDescription(ui->m_showLastBuildDescCheckBox->isChecked());
	Prefs.setShowNodes(ui->m_showNodesCheckBox->isChecked());
	Prefs.setShowNodesResponseTime(ui->m_showNodesResponseTimeCheckBox->isChecked());
	Prefs.setShowPositionInQueue(ui->m_showPositionInQueue->isChecked());

	const QComboBox *jndacb = ui->m_jobsNameDescAlignComboBox;
	Prefs.setJobsNameDescAlignFlags(jndacb->itemData(jndacb->currentIndex()).toInt());

	const QComboBox *witcb = ui->m_weatherIconsThemeComboBox;
	Prefs.setWeatherIconsTheme(witcb->itemData(witcb->currentIndex()).toString());

	const QComboBox *fcb = ui->m_fontComboBox;
	Prefs.setFont(fcb->itemData(fcb->currentIndex()).toString());

	Prefs.setRunningJobsRedrawTime(ui->m_runningJobsRedrawTimeSpinBox->value());
}
//------------------------------------------------------------------------------
