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
#include "menubar.h"
#include "actions.h"
MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent),m_FileMenu(NULL),m_SettingsMenu(NULL),m_HelpMenu(NULL)
{
	m_FileMenu = new QMenu( tr("File"),parent ) ; 
    
    m_FileMenu->addAction( Actions::getAction( Actions::scNewDownloadActionText ) ) ;
    m_FileMenu->addSeparator();
    m_FileMenu->addAction( Actions::getAction ( Actions::scHideUnHideAppActionText ) ) ;
    m_FileMenu->addAction( Actions::getAction ( Actions::scQuitActionText ) ) ;

    m_SettingsMenu = new QMenu( tr("Options"),parent );
    m_SettingsMenu->addAction( Actions::getAction( Actions::scConfigureColumnsActionText ) ) ; 
    m_SettingsMenu->addAction( Actions::getAction( Actions::scSettingsActionText ) ) ; 

    m_HelpMenu = new QMenu( tr("Help"),parent );
    m_HelpMenu->addAction( Actions::getAction( Actions::scAboutActionText ) ) ; 
    m_HelpMenu->addAction( Actions::getAction( Actions::scAboutQtActionText ) ) ; 


    addMenu(m_FileMenu);
    addMenu(m_SettingsMenu);
    addMenu(m_HelpMenu);

}
