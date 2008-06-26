#ifndef UI_USERSETTINGS_H_
#define UI_USERSETTINGS_H_

#include <QWidget>
#include <QString>
#include <memory>
#include "ui_usersettings.h"
class Ui_UserSettingsImpl : public QDialog
{
public:
	Ui_UserSettingsImpl(const QString & UserName, const QString & userPass, QWidget * parent  = 0);
	virtual ~Ui_UserSettingsImpl();
	QString						GetUser();
	QString						GetPassword();
private:
	Ui::Ui_UserSettings ui;
	std::auto_ptr<QString>		m_apUserName;
	std::auto_ptr<QString>		m_apPassword;
private slots:
	void ok();
};

#endif /*UI_USERSETTINGS_H_*/
