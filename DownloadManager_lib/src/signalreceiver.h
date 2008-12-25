/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza?a                                  *
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
#ifndef SIGNALRECIVER__H__
#define SIGNALRECIVER__H__

#include <QObject>
#include <QMap>
#include <QPair>
#include <QString>
#include "downloadmanager.h"

class ISignalListener 
{
public:
    virtual void statusChanged(int, DownloadState::States ) = 0 ; 
    virtual void progressInfoAt( int at, const ProgressInfo& _info ) = 0 ;
    ISignalListener () {};
};
class SignalPlayGround : public QObject
{
    Q_OBJECT
public:
     enum Bits
     {
         ALL = 0x1,
         NONE = 0x2,
         STATUSCHANGED = 0x4,
         PROGRESSINFOAT = 0x8
     };
     typedef QPair<Bits, ISignalListener*> BitsOfListenerPair ; 
     typedef QMap<QString, BitsOfListenerPair > MapType ; 
     ~SignalPlayGround () ; 
// private: 
//     SignalReceiver() ; 
// 
//     Bits m_bits ; 
public slots:
     virtual void statusChanged( int at, DownloadState::States state  ) ; 
     virtual void progressInfoAt( int at, const ProgressInfo& _info );
private:
    MapType m_MapOfListeners ; 
    SignalPlayGround () {} ;
    static SignalPlayGround * instance () ; 
public:
    static void connectToPlayGround( ISignalListener * listener, const QString & name, Bits mask = ALL ) ;
    static void disconnectFromPlayGround( const QString & name) ;
};
#endif //SIGNALRECIVER__H__
