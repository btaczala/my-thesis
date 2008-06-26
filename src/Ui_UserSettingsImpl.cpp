#include "Ui_UserSettingsImpl.h"

Ui_UserSettingsImpl::Ui_UserSettingsImpl(const QString & UserName, const QString & userPass, QWidget * parent) : 
	QDialog(parent), m_apUserName(new QString(UserName)), m_apPassword(new QString(userPass))
{
	ui.setupUi(this) ;
	ui.UserLabel->insert( *m_apUserName );
	ui.passwordLabel->insert( *m_apPassword );
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(ok));
}
Ui_UserSettingsImpl::~Ui_UserSettingsImpl()
{
	m_apUserName.release();
	m_apPassword.release();
}
void Ui_UserSettingsImpl::ok()
{
	m_apUserName.reset( new QString( ui.UserLabel->text() ) );
	m_apPassword.reset( new QString( ui.passwordLabel->text() ) );
}
QString Ui_UserSettingsImpl::GetUser()
{
	if( m_apUserName.get() )
		return *m_apUserName;
	return QString("");
}
QString Ui_UserSettingsImpl::GetPassword()
{
	if(m_apPassword.get())
		return *m_apPassword;
	return QString("");
}
