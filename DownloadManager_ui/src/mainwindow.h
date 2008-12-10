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
#include "systemdock.h"

/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
	@author Tomasz Czyzewski <tomasz.czy@gmail.com>
*/
//fwd declarations

class QDownloadWidget;
class MenuBar;
class QKeyEvent;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget* parent = 0);
        virtual ~MainWindow() throw() ; 

        static QStringList getLinksFromClipboard();

        void addDownload(const QString& url, const QString& fileDestination); 

    public slots:
        void about();
        void showSettingsDialog();
        void trayActivated(QSystemTrayIcon::ActivationReason reason);
        void onClose();
        void moveToTray();
        void restoreFromTray();
        void addNewDownload();
    signals:
        void signalMoveToTray();
        void signalRestoreFromTray();

    protected:
        void initializeToolbarWidget();
        void initializeDownloadWidget();
        void initializeWidgets();
        void initializeMenuBar();
        void initializeTrayIcon();
        void initializeGeometry();

        bool confirmAppExit();

        void initializeActions();
        virtual void keyPressEvent(QKeyEvent* event);
        virtual void keyReleaseEvent(QKeyEvent* event);
        virtual void closeEvent(QCloseEvent* event);
        virtual void changeEvent(QEvent* event);

    private:
        std::auto_ptr<MenuBar>          m_MenuBar; 
        std::auto_ptr<QToolBar>         m_ToolbarWidget; 
        std::auto_ptr<QDownloadWidget>  m_DownloadWidget;
        std::auto_ptr<QSystemTrayIcon>  m_trayIcon;
        bool                            m_forceExit;
        Qt::WindowStates                m_oldstate; //used by tray icon functions
        QMenu                           m_trayContextMenu;
};
#endif
