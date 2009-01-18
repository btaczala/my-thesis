/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała   *
 *   b@kontrasty.szczecin.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
//#ifdef WIN32
////#define CRTDBG_MAP_ALLOC
//#include <stdlib.h>
////#include <crtdbg.h>
//#endif // WIN32

#include <QCoreApplication>
#include "mainwindow.h"
#include <proxy.h>
#include <settings.h>
#include "singleapplication.h"

int main(int argc, char *argv[])
{	
	#ifdef WIN32
	QApplication::setStyle(new QCleanlooksStyle);
	#endif
	QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("settings"); 
    QCoreApplication::setApplicationName(Proxy::getAppName());
    QSettings::setDefaultFormat( QSettings::IniFormat ); 
    QString cp = QDir::currentPath();
    QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, cp);
    
	Q_INIT_RESOURCE(main_resources);
    Proxy::init(); 

    int iRet = 0;
    bool quitApp = false;
    bool oneInstance = Proxy::settings()->value(SettingsValNames::scOnlyOneInstanceAllowed).toBool();
    
    SingleApplication instance(Proxy::getAppUid(), &app);

    QStringList arguments = app.arguments();
    if (oneInstance && instance.isRunning())
    {
        qDebug() << "Instance is running";
        if ( arguments.count() > 1 ) 
        {
            int indexOf = arguments.indexOf( QString ("-add") );
            if ( indexOf != -1 ) 
            {
                QString url = arguments[indexOf+1] ; 
                instance.sendMessage(MainWindow::AddNewDownloadMessage + "-" + url);
            }
        }
        instance.sendMessage(MainWindow::ActivateWindowMessage);
        quitApp = true;
    }

    if (!quitApp)
    {
	    MainWindow *window = new MainWindow();
        QObject::connect(&instance, SIGNAL(messageReceived(const QString&)), window, SLOT(handleMessage(const QString&))); 
        
        if ( arguments.count() > 1 ) 
        {
            int indexOf = arguments.indexOf( QString ("-add") );
            if ( indexOf != -1 ) 
            {
                QString url = arguments[indexOf+1] ; 
                window->addDownload(url);
                instance.sendMessage(MainWindow::AddNewDownloadMessage + "-" + url);
            }
        }
        
        bool startInTray = Proxy::settings()->value(SettingsValNames::scStartInTrayArea).toBool();
        if (startInTray)
            window->moveToTray();
        else
	        window->show();

	    iRet = app.exec();
    #ifdef WIN32
	    ///_CrtDumpMemoryLeaks();
    #endif // WIN32
        delete window; 
    }
    else
    {
        qDebug() << " Application already started " ; 
    }

    Proxy::deinit();
    qDebug() << "End of App";
	return iRet;
}
