#include "systemdock.h"

SystemDock::SystemDock(QObject *parent ) : QSystemTrayIcon ( parent ) 
{
}

void SystemDock::activated( QSystemTrayIcon::ActivationReason reason )
{
    emit trayActivated(reason);
}