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
/*#ifndef ACTIONS_H
#define ACTIONS_H
#include <QAction>
#include <memory>
// only UI Actions ! ( no rsdm actions allowed ! ) 
class QWidget ; 
namespace UI
{
	struct ActionSet
	{
		ActionSet(QWidget *parent);
		~ActionSet();
		struct MenuActionSet
		{
			MenuActionSet(QWidget *parent);
			struct FileMenuActionSet
			{
				FileMenuActionSet(QWidget *parent);
				std::auto_ptr<QAction>		m_NewAct;
				std::auto_ptr<QAction>		m_SendToTrayAct;
				std::auto_ptr<QAction>		m_QuitAct;
			}m_FileMenuAct ;
			struct EditMenuActionSet
			{
				EditMenuActionSet(QWidget *parent);
				std::auto_ptr<QAction>		m_PauseSelectedAct;
				std::auto_ptr<QAction>		m_ResumeSelectedAct;
				std::auto_ptr<QAction>		m_StartSelectedAct;
			}m_EditMenuAct ;
		}m_MenuActions;
		struct ContextMenuActionSet
		{
			ContextMenuActionSet(QWidget *parent);
		}m_ContextMenuActions;
	};
};

#endif // ACTIONS_H
*/
