/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała                                  *
 *   b@kontrasty.szczecin.pl                                               *
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
#include "qrapidshareuser.h"
#include <proxy.h>
#include <settings.h>
//
const QString QRapidshareUser::ComposeCookie()
{
    QString cookie="user=";
    QString userName = Proxy::settings()->value(SettingsValNames::scPluginUsername,Settings::PLUGINS,"rapidshare").value<QString>() ;
    cookie += userName;
    cookie +="-";
    QString password = Proxy::settings()->value(SettingsValNames::scPluginPassword,Settings::PLUGINS,"rapidshare").value<QString>() ;
    password = Proxy::decrypt(password);
    QString aa = password.toAscii();
    foreach(QChar a,aa)
    {
        cookie += QString("%" + aa.sprintf("%x", a.toLatin1()) ).toUpper() ;    
    }
    return cookie;
}
//
