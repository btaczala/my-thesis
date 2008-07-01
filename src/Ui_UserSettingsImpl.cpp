#include "Ui_UserSettingsImpl.h"
Ui_UserSettingsImpl::Ui_UserSettingsImpl(QSettings * pSettings, QWidget * parent) :
	QDialog(parent)
{
	LOG_FUNC_TO_OUT ;
	ui.setupUi(this) ;
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(ok()));
	connect(ui.browseDirectory, SIGNAL(pressed()), this, SLOT( browseForDirectory() ) ) ;
	m_pSettings = const_cast< QSettings* >( pSettings );
	ReadSettings();
	m_DefaultPath.reset( new QString( m_pSettings->value( RSM_DEF_DOWNLOADS_PATH ).toString() ) );
	
}
Ui_UserSettingsImpl::~Ui_UserSettingsImpl()
{
	LOG_FUNC_TO_OUT ;
	m_apUserName.release();
	m_apPassword.release();
	m_MaxDownloads.release();
	m_DefaultPath.release();
};
void Ui_UserSettingsImpl::ReadSettings()
{
	LOG_FUNC_TO_OUT ;
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
	LOG_FUNC_TO_OUT ;
	m_apUserName.reset( new QString( ui.UserLabel->text() ) );
	m_apPassword.reset( new QString( ui.passwordLabel->text() ) );
	m_MaxDownloads.reset(new QString( ui.maxParallelDownloadsEdit->text() ) );
	m_DefaultPath.reset(new QString( ui.defaultDirectoryEdit->text() ) );
	QDialog::accept();
};
QString Ui_UserSettingsImpl::GetUser()
{
	LOG_FUNC_TO_OUT ;
	if( m_apUserName.get() )
		return *m_apUserName;
	return QString("");
};
QString Ui_UserSettingsImpl::GetPassword()
{
	LOG_FUNC_TO_OUT ;
	if( m_apPassword.get() )
		return *m_apPassword;
	return QString("");
};
QString Ui_UserSettingsImpl::GetDefaultPath()
{
	LOG_FUNC_TO_OUT ;
	if( m_DefaultPath.get() )
		return *m_DefaultPath;
	return QString( "" );
};
int Ui_UserSettingsImpl::GetMaxDownloadsNumber()
{
	LOG_FUNC_TO_OUT ;
	bool bOk = false;
	int t = 0;
	if( m_MaxDownloads.get() )
		t = m_MaxDownloads->toInt(&bOk);
	return (bOk && t) ? t : 3;
};
void Ui_UserSettingsImpl::browseForDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a destination directory"), (* m_DefaultPath ) );
	if( dir.isEmpty() )
		dir = (* m_DefaultPath );
	ui.defaultDirectoryEdit->clear();
	ui.defaultDirectoryEdit->insert( dir );
	
}
	

