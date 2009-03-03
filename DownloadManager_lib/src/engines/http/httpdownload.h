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
#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <memory>
#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QHttp>
#include <QFile>
//#include <QHttpRequestHeader>

#include "idownload.h"

class OptionsContainer;
 
class HttpDownload :  public IDownload
{
    Q_OBJECT
public:

     // Ctors:
    HttpDownload( OptionsContainer* options );
    virtual                             ~HttpDownload();
    
    void                                start(); 
    void                                stop(); // abort () 
    void                                restart(); 
 
    bool                                operator==( const HttpDownload & _cmp );
    
private:
    HttpDownload( const HttpDownload & _cpy ) ; // hide 
    HttpDownload&                       operator=( const HttpDownload& rhs );
    void                                renameFile();
private:
    QString                             m_DownloadServerHost ;
    QHttp                               m_HttpObj;
    std::auto_ptr<QHttpRequestHeader>   m_apHttpRequestHeader;

private slots:
    void                                requestStarted(const int & idReq) ;
    void                                requestFinished(const int & idReq, const bool & isFalse) ;
    void                                stateChanged(const int & iState) ;
    void                                dataSendProgress(const int & done, const int & total) ;
    void                                dataReadProgress(const int & done, const int & total) ;
    void                                responseHeaderReceived( const QHttpResponseHeader & resp) ;
    void                                done(const bool & error);
    void                                authenticationRequired ( const QString & hostname, quint16 port, QAuthenticator *authenticator );
    void                                proxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );
    void                                readyRead ( const QHttpResponseHeader & resp );
 
signals:
    void                                statusChanged( DownloadState::States status );
};
#endif //HTTPDOWNLOAD_H
//eof

