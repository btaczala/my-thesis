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
#ifndef QRAPIDSHAREDOWNLOAD_H
#define QRAPIDSHAREDOWNLOAD_H
// qrapidsharedownload - representing one download from rapidshare server
#include <QString>
#include <QHttp>
#include <QHttpRequestHeader>
#include <QUrl>
#include <QDebug>
#include <QObject>
#include <QFile>

#include "qrapidshareuser.h"
#include "rslogger.h"
#include  "idownload.h"

//errors
 
 /*! \class QRapidshareDownload - representing one download
  * \brief Class for downloading from rapidshare. Set filePath in constructor, or in Download.  
  * 
  */
 

class QRapidshareDownload :  public IDownload
{
Q_OBJECT
 public:

     enum RS_State
     {
        GET_FIRST,
        GET_SECOND,
        GET_THIRD,
        POST_FIRST,
        POST_SECOND,
        DOWNLOADING,
        FINISHED
     };
    // Ctors:
    
    QRapidshareDownload( OptionsContainer* options = NULL );
    virtual                             ~QRapidshareDownload();
    //from IDownlo
    void                                start(); 
    void                                stop(); // abort () 
    void                                restart(); 
    
    void                                setUser(const QRapidshareUser & rsUser );
    void                                setUser(const std::string& rsName,  const std::string& pass);   
    void                                setRapidshareUser(const QRapidshareUser & _usr );
    QRapidshareUser*                    getUser() const ;
 
    bool                                operator==( const QRapidshareDownload & _cmp );

    const QString                       getFullUrlFileAddress() const ; 
    const QString                       getFileDestination() const ;
    const QString                       getDownloadHost() const ; 
    void                                setDownloadHost(const QString & _host ) ;
    
 private:
    QRapidshareDownload( const QRapidshareDownload & _cpy ) ; // hide 

    virtual void                        timerEvent(QTimerEvent *event); 
    QString                             parseResponseAndGetNewUrl(const QString & resp);
    int                                 parseResponseAndGetFileSize(const QString & resp);
    QString                             parsePostReponseAndGetAddress(const QString & resp);
    void                                setUrlFileAddress(const QString & _addr ) ;
    void                                renameFile();

    bool                                checkForErrors( const QByteArray& response );
    QString                             transformUrlPathToLocalPath(const QString & url);
    QRapidshareDownload&                operator=( const QRapidshareDownload& rhs );
 private:
    QString                             m_ReferrerFileAddress;
    QString                             m_qFileDestination;
    QString                             m_DownloadServerHost ; 
    std::auto_ptr<QHttp>                m_apHttpObj;
    std::auto_ptr<QHttpRequestHeader>   m_apHttpRequestHeader;
    std::auto_ptr<QRapidshareUser>      m_apRSUser;
    bool                                m_bIsPrepared;
    std::auto_ptr<QUrl>                 m_apFileUrl;
    std::auto_ptr<QFile>                m_apFile;
    QString                             m_HostName;
    QString                             m_PathOnServer;
    QString                             m_RequestType;  
    QFile                               m_FileLog;
    int                                 m_timerId;
    qint64                              m_readedBytes;
    RSLogger                            m_Logger;
    QList<const char*>                  m_errorsList;
    RS_State                            m_rssmState;

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
    //void                                downloadStatus(const int & istate );
    void                                downloadRate(const QString & dwnlRate);
    void                                bytesRead( int read, int howMany ) ; 
 };
#endif
