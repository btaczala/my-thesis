/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczala                                  *
 *   b@kontrasty.szczecin.pl                                               *
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
#ifndef systemdock_h__
#define systemdock_h__
#include <QObject>
#include <signalreceiver.h>
#include <QSystemTrayIcon>
class SystemDock : public QObject, ISignalListener
{
	Q_OBJECT
public:
    SystemDock(QObject *parent=NULL);
	~SystemDock();
	void showTray();
	void showWidget();
	void hideTray();
	void hideWidget();
	static const QString SystemDockName ; 
	const QSystemTrayIcon * systemTrayIcon() const 
	{
		return m_pTrayIcon;
	}
private:
	QSystemTrayIcon*    m_pTrayIcon ; 
	QWidget*			m_pToolTipBig;
	QWidget*			m_pToolTipSmall;
	void				registerAtSignalPlayGround();
	void				unregisterAtSignalPlayGround();

	// from ISignalListener
	void				statusChanged(int, DownloadState::States ); 
	void				progressInfoAt( int at, const ProgressInfo& _info ) ;
	void				downloadAdded( int newPosition ) ;
	void				downloadRemoved( int newPosition ) ;
	void				initWidgets(void);
private slots:
	void				systemTrayActivated( QSystemTrayIcon::ActivationReason reason );

};
#endif // systemdock_h__