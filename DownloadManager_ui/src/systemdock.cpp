#include "systemdock.h"

#include <QSystemTrayIcon>
#include <proxy.h>
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>
#include "qdownloadwidget.h"
const QString SystemDock::SystemDockName = QString("SystemDock");

SystemDock::SystemDock(QObject *parent ) : QObject(parent), m_pTrayIcon( new QSystemTrayIcon(this)), m_pToolTipBig( new QWidget(NULL,Qt::ToolTip)),m_pDownloadManger(NULL)
{
	initWidgets();
	m_pTrayIcon->setIcon(QIcon(":/app_icon.png"));
	connect(m_pTrayIcon,SIGNAL(activated( QSystemTrayIcon::ActivationReason )),this,SLOT(systemTrayActivated( QSystemTrayIcon::ActivationReason )));
    
    m_pDownloadManger = Proxy::downloadManager() ; 
}
SystemDock::~SystemDock()
{
	delete m_pTrayIcon;
	delete m_pToolTipBig;
}

void SystemDock::initWidgets( void )
{
	QVBoxLayout* layout = new QVBoxLayout(m_pToolTipBig);
	//
    //layout->addWidget(new QDownloadWidget(m_pToolTipBig));
	m_pToolTipBig->setLayout(layout);
}

void SystemDock::showTray()
{
    m_pTrayIcon->show();
}
void SystemDock::hideTray()
{
    m_pTrayIcon->hide();
}

void SystemDock::showWidget()
{
    
    m_pToolTipBig->show();
    int x = m_pTrayIcon->geometry().x();
    int y = m_pTrayIcon->geometry().y();
    m_pToolTipBig->move(100,100);
}
void SystemDock::connectToDownloadManager() {
    connect ( m_pDownloadManger , SIGNAL ( downloadAdded(int)), this, SLOT (downloadAdded(int)) );
    connect ( m_pDownloadManger , SIGNAL ( downloadRemoved(int)), this, SLOT ( downloadRemoved(int)) );
}
void SystemDock::disconnectFromDownloadManager() {  
    disconnect ( m_pDownloadManger , SIGNAL ( downloadAdded(int)), this, SLOT (downloadAdded(int)) );
}
void SystemDock::statusChanged( int, DownloadState::States ){
}

void SystemDock::progressInfoAt( int at, const ProgressInfo& _info ){
}

void SystemDock::downloadAdded( int newPosition ){
    //const IDownload *pDownload = m_pDownloadManger->downloadAt(newPosition);
    m_pTrayIcon->setToolTip( QString("Download added at %1").arg(newPosition) );
}

void SystemDock::downloadRemoved( int newPosition ){
    m_pTrayIcon->setToolTip( QString("Download added at %1").arg(newPosition) );
}

void SystemDock::systemTrayActivated( QSystemTrayIcon::ActivationReason reason ){
	if ( m_pToolTipBig->isVisible() )
		m_pToolTipBig->hide();
	else
		showWidget();
}

