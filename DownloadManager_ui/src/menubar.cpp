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
UI::MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent), m_SettingsMenu(parent), m_FileMenu(parent)
{
	addMenu(m_FileMenu.m_Menu.get());
	addMenu(m_SettingsMenu.m_Menu.get());	
};
UI::MenuBar::FileMenuActions::FileMenuActions(QWidget * parent) :
		m_Menu(new QMenu(tr("File"),parent)), m_NewAct(new QAction(tr("New download"),m_Menu.get())), m_SendToTrayAct(new QAction(tr("Send to tray"),m_Menu.get())), m_ExitAct(new QAction(tr("Quit"),m_Menu.get()))
{
	m_Menu->addAction(m_NewAct.get());
	m_Menu->addSeparator();
	m_Menu->addAction(m_SendToTrayAct.get());
	m_Menu->addAction(m_ExitAct.get());
	
}
UI::MenuBar::SettingMenuActions::SettingMenuActions(QWidget * parent) : m_Menu( new QMenu(tr("Settings"), parent)), m_ConfigureAct(new QAction(tr("Configure"), m_Menu.get()))
{
	m_Menu->addAction(m_ConfigureAct.get());
}