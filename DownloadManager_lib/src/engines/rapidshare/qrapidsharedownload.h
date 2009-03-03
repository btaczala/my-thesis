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
 
 namespace RsErrors
{
//<!-- E#5 -->
static const char* err5= "Due to a violation of our terms of use, the file has been removed from the server";
//<!-- E#2 -->    
static const char* err2 = "The Account has been found, but the password is incorrect";
}

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
 
    bool                                operator==( const QRapidshareDownload & _cmp );

    const QString                       getFullUrlFileAddress() const ; 
    const QString                       getFileDestination() const ;
    const QString                       getDownloadHost() const ; 
    void                                setDownloadHost(const QString & _host ) ;
    
 private:
    QRapidshareDownload( const QRapidshareDownload & _cpy ) ; // hide 

    QString                             parseResponseAndGetNewUrl(const QString & resp);
    qint64                              parseResponseAndGetFileSize(const QString & resp);
    QString                             parsePostReponseAndGetAddress(const QString & resp);
    void                                setUrlFileAddress(const QString & _addr ) ;

    bool                                checkForErrors( const QByteArray& response );
    QString                             transformUrlPathToLocalPath(const QString & url);
    
    void                                connectToQHttp() ; 
    void                                disconnectFromQHttp() ; 
    
    void                                setFailed();
    void                                setStopped();
    void                                setPaused();
    
    QRapidshareDownload&                operator=( const QRapidshareDownload& rhs );
 private:
    QString                             m_ReferrerFileAddress;
    QString                             m_DownloadServerHost ; 
    std::auto_ptr<QHttp>                m_apHttpObj;
    std::auto_ptr<QHttpRequestHeader>   m_apHttpRequestHeader;

    bool                                m_bIsPrepared;
    std::auto_ptr<QUrl>                 m_apFileUrl;
    QString                             m_HostName;
    QString                             m_PathOnServer;
    QString                             m_RequestType;  
//     QFile                               m_FileLog;
    QList<const char*>                  m_errorsList;
    RS_State                            m_rssmState;
    qint64                              m_iPDownloaded;

 private slots:
    void                                requestFinished(const int & idReq, const bool & isFalse) ;
    void                                dataReadProgress(const int & done, const int & total) ;
    void                                responseHeaderReceived( const QHttpResponseHeader & resp) ;
    void                                done(const bool & error);
 
signals:
    //void                                statusChanged( DownloadState::States status );
    //void                                downloadStatus(const int & istate );
    //void                                downloadRate(const QString & dwnlRate);
    //void                                bytesRead( int read, int howMany ) ; 
    //void                                elapsedTime( unsigned int elapsedTime ) ; 
 };
#endif
