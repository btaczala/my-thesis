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
#ifndef MENUBAR_H
#define MENUBAR_H
#include <QMenuBar>
#include <memory>
namespace UI
{
  class MenuBar : public QMenuBar
  {
	  Q_OBJECT
  public:
	  MenuBar(QWidget *parent );
	  struct FileMenu
	  {
		  std::auto_ptr<QMenu>		m_Menu ; 
		  std::auto_ptr<QAction>		m_NewAct;
		  std::auto_ptr<QAction>		m_SendToTrayAct;
		  std::auto_ptr<QAction>		m_ExitAct;
	  }m_FileMenu;

	  struct SettingMenu
	  {
		  std::auto_ptr<QMenu>		m_Menu ;
		  std::auto_ptr<QAction>		m_ConfigureAct ;
	  }m_SettingsMenu;
  protected:
	  //virtual void paintEvent(QPaintEvent *event);
  };
};
#endif // MENUBAR_H