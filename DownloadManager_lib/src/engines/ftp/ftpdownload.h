/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza�a                                 *
 *   b@kontrasty.szczecin.pl                                                  *
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

#ifndef FTPDOWNLOAD_H
#define FTPDOWNLOAD_H

#include <QFtp>
#include <QUrl>
#include <QFile>

#include "idownload.h"

class OptionsContainer;


class FtpDownload : public IDownload
{
Q_OBJECT
public:

    enum FtpState
    {
        
    };
    
    // Ctors:
    FtpDownload( OptionsContainer* options );
    virtual                             ~FtpDownload();
    
    void                                start(); 
    void                                stop(); // abort () 
    void                                restart(); 
    
    bool                                operator==( const FtpDownload & _cmp );
    
private:
    FtpDownload( const FtpDownload & _cpy ) ; // hide 
    FtpDownload&                        operator=( const FtpDownload& rhs );
    void                                ftpDisconect();
    void                                setConnection();
    void                                beginDownload();
    void                                ftpLogin();
    void                                makeCdOrDownload();
    QString                             nextDir();
    
private:
    QString                             m_DownloadServerHost ;
    std::auto_ptr<QFtp>                 m_apFtpObj;
    std::auto_ptr<QUrl>                 m_apHost;
    qint64                              m_readedBytes;
    int                                 m_timerId;
    QString                             m_user;
    QString                             m_pass;
    QString                             m_path;
    QString                             m_FileName;
    
    private slots:
    void ftpCommandFinished ( int id, bool error );
    void ftpDataTransferProgress ( qint64 done, qint64 total );
    void ftpDone ( bool error );
    void ftpStateChanged ( int state );
      
signals:
    void                                statusChanged( DownloadState::States status );
    void                                downloadRate(const QString & dwnlRate);
    void                                bytesRead( int read, int howMany ) ;           
    void                                elapsedTime( unsigned int elapsedTime ); 
};

#endif 


