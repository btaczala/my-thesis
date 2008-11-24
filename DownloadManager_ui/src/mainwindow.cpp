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
#include <QtGui>
#include "mainwindow.h"
#include "menubar.h"
#include "qdownloadwidget.h"
#include "actions.h"

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent), m_MenuBar(new MenuBar(this)), m_DownloadWidget(new QDownloadWidget(this)),
     m_ToolbarWidget(new QToolBar("Download Toolbar", this))
{
	QMenuBar *menuBar = dynamic_cast<QMenuBar*>(m_MenuBar.get());
	setMenuBar(menuBar);

    InitializeWidgets();
    
    connect( Actions::getAction(Actions::scQuitActionText), SIGNAL( triggered() ), this, SLOT( close() ) ) ;   
    connect( Actions::getAction(Actions::scStartRestoreActionText),SIGNAL( triggered() ),this,SLOT( StartResume() ) );
};

MainWindow::~MainWindow() throw()
{
}

void MainWindow::InitializeWidgets()
{
    InitilizeToolbarWidget();
    InitilizeDownloadWidget();

    QRect rect = QApplication::desktop()->geometry();

    int x = static_cast<int>(rect.x() + rect.width() * 1/8);
    int y = static_cast<int>(rect.y() + rect.height() * 1/8);
    int w = static_cast<int>(rect.width() * 3/4);
    int h = static_cast<int>(rect.height() * 3/4);

    setGeometry(x, y, w, h);
}

void MainWindow::InitilizeToolbarWidget()
{
    m_ToolbarWidget->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_ToolbarWidget->setFloatable(false);
    m_ToolbarWidget->setMovable(false);
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scNewActionText ) );
    m_ToolbarWidget->addSeparator();
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scStartRestoreActionText ) );
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scStopActionText ) );
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scRemoveActionText ) );
    m_ToolbarWidget->addSeparator();
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scQuitActionText ) );

    addToolBar(m_ToolbarWidget.get());
}

void MainWindow::InitilizeDownloadWidget()
{
    setCentralWidget(m_DownloadWidget.get());
}
////////////////////////// slots /////////////////////////////////
void MainWindow::StartResume()
{
}

