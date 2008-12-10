/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała 				   *
 *   b@kontrasty.szczecin.pl   						   *
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
#ifndef ACTIONS_H
#define ACTIONS_H
#include <QAction>
#include <QMap>
#include <QObject>
#include <boost/shared_ptr.hpp>
class Actions
{
public:
    typedef boost::shared_ptr<QAction>    QActionShPtr; 
    typedef QMap<QString,QActionShPtr >   MapType; 
    static QAction* getAction(const QString & actionName ); 

    static const QString scNewDownloadActionText; 
    static const QString scStartRestoreActionText; 
    static const QString scStopActionText; 
    static const QString scRemoveActionText; 
    static const QString scQuitActionText; 
    static const QString scHideUnHideAppActionText; 
    static const QString scConfigureColumnsActionText;
    static const QString scSettingsActionText;
    static const QString scAboutActionText;
    static const QString scAboutQtActionText;
    static const QString scHideCurrentColumnText;

private:
    static const Actions* instance() ; 
    QAction* action(const QString & actionName ) const ; 
    Actions(const Actions & ) ; 
    Actions() ; 
    Actions & operator=(const Actions & rhs) ; 
    MapType     m_ActionContainer ; 
};
#endif // ACTIONS_H

