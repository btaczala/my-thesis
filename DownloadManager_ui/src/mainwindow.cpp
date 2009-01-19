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
#include <QKeyEvent>
#include "mainwindow.h"
#include "menubar.h"
#include "qdownloadwidget.h"
#include "actions.h"
#include "settings_ui/settingsdialog.h"
#include "adddownloaddialog.h"

#include <proxy.h>
#include <settings.h>

const QString MainWindow::ActivateWindowMessage = "ActivateWindowMessage";
const QString MainWindow::AddNewDownloadMessage = "AddNewDownloadMessage";

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent), m_MenuBar(new MenuBar(this)), m_DownloadWidget(new QDownloadWidget(this)),
    m_ToolbarWidget(new QToolBar("Download Toolbar", this)), /*m_trayIcon( new QSystemTrayIcon(this)),*/m_pSystemDock(new SystemDock(this))
{
    m_forceExit = false;

    initializeWidgets();
    initializeActions();
    
    setWindowTitle("Rapidshare download manager ") ; 
    setWindowIcon(QIcon(":/app_icon.png"));
}

MainWindow::~MainWindow() throw()
{
    Proxy::settings()->setValue(SettingsValNames::scMainWindowGeometry, saveGeometry(), Settings::UI);
}

QStringList MainWindow::getLinksFromClipboard()
{
    QClipboard* clipboard = QApplication::clipboard();
    QString text = clipboard->text(QClipboard::Clipboard);
    QStringList urls = text.split( QRegExp("\\s+") );

    return urls;
}

void MainWindow::addDownload(const QString& url, const QString& fileDestination)
{
    QString dest ; 
    if ( fileDestination.isEmpty() ) 
        dest = Proxy::settings()->value(SettingsValNames::scDefaultDownloadDirectory ).toString() ; 
    else 
        dest = fileDestination ; 
    m_DownloadWidget->addDownload( url, dest );
}

void MainWindow::bringWindowToFront()
{
    show();
    setWindowState(windowState() & ~Qt::WindowMinimized);
	raise();
	activateWindow();
}

void MainWindow::initializeWidgets()
{
    initializeMenuBar();
    initializeToolbarWidget();
    initializeDownloadWidget();
    //initializeTrayIcon();
    initializeGeometry();
}

void MainWindow::initializeMenuBar()
{
    QMenuBar *menuBar = qobject_cast<QMenuBar*>(m_MenuBar);
	setMenuBar(menuBar);
}

void MainWindow::initializeToolbarWidget()
{
    m_ToolbarWidget->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_ToolbarWidget->setFloatable(false);
    m_ToolbarWidget->setMovable(false);
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scNewDownloadActionText ) );
    m_ToolbarWidget->addSeparator();
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scStartRestoreActionText ) );
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scStopActionText ) );
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scRemoveActionText ) );
    m_ToolbarWidget->addSeparator();
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scSettingsActionText ) );
    m_ToolbarWidget->addSeparator();
    m_ToolbarWidget->addAction( Actions::getAction( Actions::scQuitActionText ) );

    m_ToolbarWidget->setIconSize(QSize(48,48));

    addToolBar(m_ToolbarWidget);
}

void MainWindow::initializeDownloadWidget()
{
    setCentralWidget(m_DownloadWidget);
}
/*
void MainWindow::initializeTrayIcon()
{
    m_trayIcon->setIcon(QIcon(":/app_icon.png"));
    m_trayContextMenu.addAction(tr("Restore"));
    m_trayIcon->setContextMenu(&m_trayContextMenu);
}
*/

void MainWindow::initializeGeometry()
{   

    QByteArray storedGeometry(Proxy::settings()->value(SettingsValNames::scMainWindowGeometry, Settings::UI).toByteArray());
    
    if (storedGeometry.isEmpty())
    {
        //load default         
        QRect rect = QApplication::desktop()->geometry();

        rect.setX(static_cast<int>(rect.x() + rect.width() * 1/8));
        rect.setY(static_cast<int>(rect.y() + rect.height() * 1/8));
        rect.setWidth(static_cast<int>(rect.width() * 5/6));
        rect.setHeight(static_cast<int>(rect.height() * 3/4));

        setGeometry(rect);
    }
    else
    {
        restoreGeometry(storedGeometry);
    }    
}

bool MainWindow::confirmAppExit()
{
    bool confirmAppExit = Proxy::settings()->value(SettingsValNames::scConfirmAppExit).value<bool>();

    if (!confirmAppExit)
        return true;

    QMessageBox msg(QMessageBox::Question, tr("Exit confirm"), tr("Are you sure you want to quit?"), 
        QMessageBox::Yes | QMessageBox::No, this);

    msg.setDefaultButton(QMessageBox::No);
    
    return (msg.exec() == QMessageBox::Yes);
}

void MainWindow::moveToTray()
{
	m_pSystemDock->showTray();
    //m_trayIcon->show();
    hide();
    //connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason
}

void MainWindow::restoreFromTray()
{
    bringWindowToFront();
    
    if (m_pSystemDock->systemTrayIcon()->isVisible())
    {
        m_pSystemDock->hideTray();
        //disconnect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
    }
}

void MainWindow::addNewDownload()
{
    AddDownloadDialog dlg(this);

    dlg.exec();
}

void MainWindow::handleMessage(const QString& message)
{
    qDebug() << Q_FUNC_INFO << "message="<< message ; 
    if (message == ActivateWindowMessage)
        restoreFromTray();
    if ( message.contains(AddNewDownloadMessage) )
    {
        QStringList a = message.split('-');
        QString url = a[1];
        addDownload( url );
    }
}

void MainWindow::initializeActions()
{
    connect( Actions::getAction(Actions::scQuitActionText), SIGNAL( triggered() ), this, SLOT( onClose() ) ) ; 
    connect( Actions::getAction(Actions::scAboutActionText), SIGNAL( triggered() ), this, SLOT( about() ) ) ; 
    connect( Actions::getAction(Actions::scAboutQtActionText), SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) ) ; 
    connect( Actions::getAction(Actions::scSettingsActionText), SIGNAL( triggered()), this, SLOT(showSettingsDialog()));
    
    connect( Actions::getAction(Actions::scNewDownloadActionText), SIGNAL(triggered()), this, SLOT(addNewDownload()));
    connect( Actions::getAction(Actions::scStopActionText), SIGNAL( triggered() ),m_DownloadWidget ,SLOT( StopSelectedDownload() ));
    connect( Actions::getAction(Actions::scStartRestoreActionText), SIGNAL( triggered() ), m_DownloadWidget, SLOT( StartPauseSelectedDownload() ) );
    connect( Actions::getAction(Actions::scRemoveActionText), SIGNAL(triggered()), m_DownloadWidget, SLOT(RemoveSelectedDownload() ) );

    connect( this, SIGNAL(signalMoveToTray()), this, SLOT(moveToTray()), Qt::QueuedConnection);
    connect( this, SIGNAL(signalRestoreFromTray()), this, SLOT(restoreFromTray()), Qt::QueuedConnection);
}

void MainWindow::showSettingsDialog()
{
    SettingsDialog dialog(this);

    dialog.exec();
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason & QSystemTrayIcon::Trigger)
    {
        emit restoreFromTray();
    }
}

void MainWindow::onClose()
{
    m_forceExit = true;
    close();
}

void MainWindow::about()
{
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/app_icon.png"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);

    QString dialogTitle(QString("%1 - %2").arg(Proxy::getAppName()).arg(Proxy::getAppVersion()));

    QString appInfo(
        QString(tr("<p><b>%1 %2 %3 </b> <br/> "
          "  This program is free software; you can redistribute it and/or modify "
          " it under the terms of the GNU General Public License as published by "
          " the Free Software Foundation; either version 2 of the License, or "
          " (at your option) any later version.</p>"
          " <p>Application icons has been downloaded from sites: <br> "
          " <a href=\"http://www.dryicons.com\">http://www.dryicons.com</a><br/> "
          " <a href=\"http://www.freeiconsweb.com\">http://www.freeiconsweb.com</a></p> " )).arg(Proxy::getAppCompany()).arg(Proxy::getAppName()).arg(Proxy::getAppVersion()));
    text->setText(appInfo);

    QPushButton *quitButton = new QPushButton("OK");

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setMargin(10);
    topLayout->setSpacing(10);
    topLayout->addWidget(icon);
    topLayout->addWidget(text);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(quitButton);
    bottomLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    QDialog about(this);
    about.setModal(true);
    about.setWindowTitle(dialogTitle);
    about.setLayout(mainLayout);

    connect(quitButton, SIGNAL(clicked()), &about, SLOT(close()));

    about.exec();
}

void MainWindow::keyPressEvent(QKeyEvent* event) 
{
    if ( event->key() == Qt::Key_V && event->modifiers() == Qt::ControlModifier )
    {   
        QStringList urls = MainWindow::getLinksFromClipboard();
        Q_FOREACH(QString one, urls ) 
        {
            addDownload(one, Proxy::settings()->value(SettingsValNames::scDefaultDownloadDirectory).toString());
        }        
    }
    else if (event->key() == Qt::Key_F2)
    {
        addNewDownload();
    }
    else if (event->key() == Qt::Key_F6)
    {
        showSettingsDialog();
    }
    else if (event->key() == Qt::Key_F10)
    {
        m_forceExit = true;
        close();
    }
    else if (event->modifiers() == Qt::ShiftModifier)
    {
        m_forceExit = true;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::ShiftModifier)
    {
        m_forceExit = false;
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    bool close2Tray = Proxy::settings()->value(SettingsValNames::scClose2Tray).value<bool>();

    if (!m_forceExit && close2Tray)
    {
        moveToTray();
        event->ignore();
    }
    else if (confirmAppExit())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent* e = static_cast<QWindowStateChangeEvent*>(event);
        unsigned int state = windowState();
 	    if (state & Qt::WindowMinimized)
        {
            bool minimize2Tray = Proxy::settings()->value(SettingsValNames::scMinimize2Tray).value<bool>();
            
            if (minimize2Tray)
            {
                emit signalMoveToTray();
            }
        }
    }

    QMainWindow::changeEvent(event);
}
