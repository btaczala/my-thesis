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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QMainWindow>
#include <memory>
#include "qdownloadwidget.h"
#include "menubar.h"
/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
*/
class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(QWidget * parent = 0);
		~MainWindow() throw() ; 
	private:
		std::auto_ptr<UI::MenuBar>			m_MenuBar ; 
		std::auto_ptr<QDownloadWidget>			m_DownloadWidget;
};
#endif