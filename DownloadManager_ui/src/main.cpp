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
#ifdef WIN32
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // WIN32

#include <QCoreApplication>
#include "mainwindow.h"
#include <proxy.h>
#include <settings.h>
int main(int argc, char *argv[])
{	
	#ifdef WIN32
	QApplication::setStyle(new QCleanlooksStyle);
	#endif
	QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("settings"); // this is no organization name, but name of settings folder. 
    QCoreApplication::setApplicationName("QRapidshare");
    QSettings::setDefaultFormat( QSettings::IniFormat ) ; 
    QString cp = QDir::currentPath() ;
    QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, cp);
    
	Q_INIT_RESOURCE(main_resources);
    Proxy::init() ; 
	MainWindow *window = new MainWindow();
	window->show();
	int iRet = app.exec();
#ifdef WIN32
	///_CrtDumpMemoryLeaks();
#endif // WIN32
    delete window; 
    Proxy::deinit();
    qDebug() << "End of App";
	return iRet;
}
