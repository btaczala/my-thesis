/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczala 								   *
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

#include <QUrl>
#include <QDebug>

#include "httpdownload.h"

HttpDownload::HttpDownload(OptionsContainer* options): IDownload(options)
, m_apHttpObj( new QHttp() )
, m_apHttpRequestHeader(new QHttpRequestHeader() )
, m_apFile(new QFile() )
{
	
    QObject::connect( m_apHttpObj.get(), SIGNAL( requestStarted( int ) ), this, SLOT( requestStarted( int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( stateChanged( int ) ), this, SLOT( stateChanged( int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( dataSendProgress( int,int ) ), this, SLOT(  dataSendProgress( int,int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( done( bool ) ), this, SLOT(  done( bool ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( authenticationRequired(  const QString , quint16 , QAuthenticator *) ), this, SLOT(  authenticationRequired(  const QString , quint16 , QAuthenticator *)  ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ), this, SLOT(  proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ) );
    QObject::connect( m_apHttpObj.get(), SIGNAL( readyRead ( QHttpResponseHeader ) ), this, SLOT(  readyRead ( QHttpResponseHeader ) ) );   
	
}

HttpDownload::~HttpDownload()
{
    m_apHttpObj.get()->disconnect();
}

void HttpDownload::start()
{
    // FIXME: ok it mean init but as far as qrapidshare engine should be aware. for IDownload it should be DownloadState::Download
    QString tmpName( m_FileDestination.c_str() );
    tmpName += "/";
    tmpName += m_FileName.c_str();
    tmpName += ".part";
    
    m_apFile->setFileName( tmpName );
    qDebug() << tmpName;
    m_pDownloadInfo->m_State = DownloadState::DOWNLOADING;
    emit statusChanged( m_pDownloadInfo->m_State );
    QUrl url(m_UrlAddress.c_str());
	
    m_apHttpRequestHeader->setRequest("GET", url.path() );
    m_apHttpRequestHeader->setValue("Host",  url.host() );
    m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
    m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50");
    m_apHttpObj->setHost(  url.host() );
    m_apHttpObj->request( *( m_apHttpRequestHeader ) );
    
}

void HttpDownload::stop()
{
	if( m_pDownloadInfo->m_State  == DownloadState::STOPPED
       || m_pDownloadInfo->m_State  == DownloadState::DONE
       || m_pDownloadInfo->m_State  == DownloadState::FAILED)
        return ; 
    m_pDownloadInfo->m_State  = DownloadState::PAUSED ;
    m_apHttpObj->abort();
    emit statusChanged( m_pDownloadInfo->m_State );
}
void HttpDownload::restart()
{
	//\fixme: implement restart
}

void HttpDownload::requestStarted(const int & idReq)
{
	qDebug("requestStarted");
}

void HttpDownload::requestFinished(const int & idReq, const bool & isFalse)
{
	qDebug("requestFinished");
    m_apFile->close();
    if( isFalse )
    {
        qDebug() << m_apHttpObj->errorString() ;
        return ; 
    }
    
}
void HttpDownload::stateChanged(const int & iState)
{
	qDebug() << "stateChanged"<< "iState =  " << iState ;
}
void HttpDownload::dataSendProgress(const int & done, const int & total)
{
	qDebug() << "stateChanged"<< "done =  " << done << " total " << total ;
}

void HttpDownload::dataReadProgress(const int & done, const int & total)
{
	qDebug() << "dataReadProgress"<< "done =  " << done << " total " << total ;
    
    // support for Direct downloads
    char *buff = NULL ; 
    qint64 iBytes2 = 0;
    qint64 bytes = m_apHttpObj->bytesAvailable();
    m_readedBytes+=bytes;
    
    int bytesDownloadedOverall = (m_pDownloadInfo->m_DownloadFileSize - total) > 0 ?  m_pDownloadInfo->m_DownloadFileSize - total : 0 ; 
    m_pDownloadInfo->m_BytesDownloaded = done + bytesDownloadedOverall; 
    double dDone = m_pDownloadInfo->m_BytesDownloaded;
    double dTotal = m_pDownloadInfo->m_DownloadFileSize;
    double dResTotal = dDone / dTotal;
    dResTotal *= 100;
    m_Progress = (int)dResTotal;
    qDebug() << "emit :DownloadStatus("<< m_Progress <<")";  
    emit bytesRead(dDone,dTotal) ;
    m_pDownloadInfo->bytesReadPreviously =m_pDownloadInfo->bytesReadCurrent;
    m_pDownloadInfo->bytesReadCurrent = done ; 
    buff = new char[bytes];
    iBytes2 = m_apHttpObj->read(buff, bytes);
    if ( -1 == iBytes2)
        qDebug()<<("ERROR while reading from Http stream ");
    else
    {
        qDebug() << "readed bytes " << iBytes2;
        if( !m_apFile->isOpen())
        {
            qDebug() << ("isClosed, open it");
            if( ! m_apFile->open(QIODevice::WriteOnly | QIODevice::Append) )
            {
                qDebug()<<("unable to open file '-_- "); //. 
                // message = bad
                // 
                return ;
            }
            else
                qDebug() << "opened";
        }
        
        qint64 btmp = m_apFile->write(buff,iBytes2); 
        if( -1 == btmp )
        {
            qDebug()<<("write failed");
            return;
        }
        else
            qDebug() << ("written successful") << btmp;
    }
    delete[] buff;
    
    if( done == total ) 
    {
        m_pDownloadInfo->m_State = DownloadState::DONE;
    }
    
}

void HttpDownload::responseHeaderReceived( const QHttpResponseHeader & resp)
{
	
}

void HttpDownload::done(const bool & error)
{
	if( error )
    {
        qDebug() << m_apHttpObj->errorString();
        return ;
    }
    
    if( m_pDownloadInfo->m_State == DownloadState::DONE )
    {   
        m_apFile->close();
        renameFile();
        emit statusChanged( m_pDownloadInfo->m_State );
       //killTimer(m_timerId);
    }
    
}

void HttpDownload::authenticationRequired ( const QString & hostname, quint16 port, QAuthenticator *authenticator )
{
	
}

void HttpDownload::proxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator )
{
	
}

void HttpDownload::readyRead ( const QHttpResponseHeader & resp )
{
	
}

void HttpDownload::renameFile()
{
    if(m_apFile.get()!=NULL )
    {
        QString tmp( m_apFile->fileName() );
        tmp = tmp.left(tmp.indexOf(".part"));
        QFile::rename(m_apFile->fileName(), tmp );
        qDebug() << tmp ;
    }
}
