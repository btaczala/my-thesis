#ifndef UI_USERSETTINGS_H_
#define UI_USERSETTINGS_H_

#include <QWidget>
#include <QString>
#include <QSettings>
#include <memory>
#include "ui_usersettings.h"
#include "common.h"
class Ui_UserSettingsImpl : public QDialog
{
	Q_OBJECT
public:
	Ui_UserSettingsImpl(const QString & UserName, const QString & userPass, QWidget * parent  = 0);
	Ui_UserSettingsImpl(QSettings * pSettings, QWidget * parent = 0);
	virtual ~Ui_UserSettingsImpl();
	QString						GetUser();
	QString						GetPassword();
	int							GetMaxDownloadsNumber();
	QString						GetDefaultPath();
private:
	Ui::Ui_UserSettings ui;
	std::auto_ptr<QString>		m_apUserName;
	std::auto_ptr<QString>		m_apPassword;
	std::auto_ptr<QString>		m_MaxDownloads;
	std::auto_ptr<QString>		m_DefaultPath;
	QSettings *					m_pSettings;
	void						ReadSettings();
	
private slots:
	void ok();
};

#endif /*UI_USERSETTINGS_H_*/
