/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała 								   *
 *   b@kontrasty.szczecin.pl   											   *
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
#include "actions.h"
#include <memory>

const QString Actions::scNewActionText = QObject::tr("New") ;
const QString Actions::scStartRestoreActionText  = QObject::tr("Start\\Resume") ;
const QString Actions::scStopActionText = QObject::tr("Stop") ;
const QString Actions::scRemoveActionText = QObject::tr("Remove") ;

const Actions* Actions::instance()
{
    static std::auto_ptr<Actions> ret( new Actions() ) ; 
    return ret.get() ; 
};
Actions::Actions()
{
    // the actions will be orphans.
    m_ActionContainer[scNewActionText] = QActionShPtr( new QAction( scNewActionText,NULL) );
    m_ActionContainer[scStartRestoreActionText] = QActionShPtr( new QAction( scStartRestoreActionText,NULL) );
    m_ActionContainer[scStopActionText] = QActionShPtr( new QAction( scStopActionText,NULL) );
    m_ActionContainer[scRemoveActionText] = QActionShPtr( new QAction( scRemoveActionText,NULL) );

}
QAction* Actions::getAction(const QString &actionName) 
{
    const Actions* pActContainer = Actions::instance() ; 
    return pActContainer->action( actionName ) ;
    
}
QAction* Actions::action(const QString &actionName) const 
{
    MapType::iterator it  = m_ActionContainer.find(actionName) ; 
    if ( it != m_ActionContainer.end() ) 
        return it->get() ; 
    return NULL ; 
}