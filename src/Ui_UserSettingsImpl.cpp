#include "Ui_UserSettingsImpl.h"

Ui_UserSettingsImpl::Ui_UserSettingsImpl(const QString & UserName, const QString & userPass, QWidget * parent) : 
	QDialog(parent), m_apUserName(new QString(UserName)), m_apPassword(new QString(userPass))
{
	ui.setupUi(this) ;
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(ok));
}
Ui_UserSettingsImpl::Ui_UserSettingsImpl(QSettings * pSettings, QWidget * parent) :
	QDialog(parent)
{
	ui.setupUi(this) ;
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(ok));
	m_pSettings = const_cast< QSettings* >( pSettings );
	ReadSettings();
}
Ui_UserSettingsImpl::~Ui_UserSettingsImpl()
{
	m_apUserName.release();
	m_apPassword.release();
	m_MaxDownloads.release();
	m_DefaultPath.release();
};
void Ui_UserSettingsImpl::ReadSettings()
{
	ui.passwordLabel->insert( m_pSettings->value(SET_USERPASSWORD).toString() );
	ui.UserLabel->insert( m_pSettings->value(SET_USERNAME).toString() );
	QString max = m_pSettings->value(RSM_MAX_DOWNLOAD).toString();
	if(max.isEmpty())
		ui.maxParallelDownloadsEdit->insert(QString::number(3));
	else
		ui.maxParallelDownloadsEdit->insert( max );
	max = m_pSettings->value( RSM_DEF_DOWNLOADS_PATH ).toString();
	if(max.isEmpty())
		ui.defaultDirectoryEdit->insert( QDir::homePath() );
	else
		ui.defaultDirectoryEdit->insert( max );
};
void Ui_UserSettingsImpl::ok()
{
	m_apUserName.reset( new QString( ui.UserLabel->text() ) );
	m_apPassword.reset( new QString( ui.passwordLabel->text() ) );
	m_MaxDownloads.reset(new QString( ui.maxParallelDownloadsEdit->text() ) );
	m_DefaultPath.reset(new QString( ui.defaultDirectoryEdit->text() ) );
	QDialog::accept();
};
QString Ui_UserSettingsImpl::GetUser()
{
	if( m_apUserName.get() )
		return *m_apUserName;
	return QString("");
};
QString Ui_UserSettingsImpl::GetPassword()
{
	if( m_apPassword.get() )
		return *m_apPassword;
	return QString("");
};
QString Ui_UserSettingsImpl::GetDefaultPath()
{
	if( m_DefaultPath.get() )
		return *m_DefaultPath;
	return QString( "" );
};
int Ui_UserSettingsImpl::GetMaxDownloadsNumber()
{
	if( m_MaxDownloads.get() )
		return m_DefaultPath->toInt();
	return 3;
};
