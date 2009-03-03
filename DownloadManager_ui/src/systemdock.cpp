#include "systemdock.h"

#include <QSystemTrayIcon>
#include <proxy.h>
#include <signalreceiverfactory.h>
#include <QWidget>
#include <QLayout>
#include "qdownloadwidget.h"
const QString SystemDock::SystemDockName = QString("SystemDock");

SystemDock::SystemDock(QObject *parent ) : QObject(parent), m_pTrayIcon( new QSystemTrayIcon(this)), m_pToolTipBig( new QWidget(NULL,Qt::ToolTip))
{
	initWidgets();
	m_pTrayIcon->setIcon(QIcon(":/app_icon.png"));
	connect(m_pTrayIcon,SIGNAL(activated( QSystemTrayIcon::ActivationReason )),this,SLOT(systemTrayActivated( QSystemTrayIcon::ActivationReason )));
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

void SystemDock::statusChanged( int, DownloadState::States )
{
}

void SystemDock::progressInfoAt( int at, const ProgressInfo& _info )
{
}

void SystemDock::downloadAdded( int newPosition )
{
}

void SystemDock::downloadRemoved( int newPosition )
{
}

void SystemDock::systemTrayActivated( QSystemTrayIcon::ActivationReason reason )
{
	if ( m_pToolTipBig->isVisible() )
		m_pToolTipBig->hide();
	else
		showWidget();
}

