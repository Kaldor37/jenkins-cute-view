//------------------------------------------------------------------------------
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H
//------------------------------------------------------------------------------
#include <QDialog>
//------------------------------------------------------------------------------
class QIntValidator;
class QAbstractButton;
class QStringList;
//------------------------------------------------------------------------------
namespace Ui {
	class PreferencesDialog;
}
//------------------------------------------------------------------------------
class PreferencesDialog : public QDialog{
		Q_OBJECT
//------------------------------------------------------------------------------
// Constructor/Destructor
//------------------------------------------------------------------------------
	public:
		explicit PreferencesDialog(QWidget *parent = nullptr);
		~PreferencesDialog();

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void viewsList_updated(const QStringList &viewsList, const QString &primaryView);

//------------------------------------------------------------------------------
// Protected functions
//------------------------------------------------------------------------------
	protected:
		virtual void showEvent(QShowEvent * event);

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
	private slots:
		void on_m_buttonBox_clicked(QAbstractButton *btn);

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void savePreferences();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		Ui::PreferencesDialog *ui;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // PREFERENCESDIALOG_H
//------------------------------------------------------------------------------
