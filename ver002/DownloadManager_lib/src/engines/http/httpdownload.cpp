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
#include "rslogger.h"

HttpDownload::HttpDownload(OptionsContainer* options)
: IDownload(options)
, m_HttpObj( new QHttp() )
, m_apHttpRequestHeader(new QHttpRequestHeader() )
{
	
    QObject::connect( &m_HttpObj, SIGNAL( requestStarted( int ) ), this, SLOT( requestStarted( int ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( requestFinished( int,bool ) ), this, SLOT( requestFinished( int,bool ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( stateChanged( int ) ), this, SLOT( stateChanged( int ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( dataSendProgress( int,int ) ), this, SLOT(  dataSendProgress( int,int ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( responseHeaderReceived( const QHttpResponseHeader & ) ), this, SLOT(  responseHeaderReceived( const QHttpResponseHeader & ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( dataReadProgress( int,int ) ), this, SLOT(  dataReadProgress( int,int ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( done( bool ) ), this, SLOT(  done( bool ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( authenticationRequired(  const QString , quint16 , QAuthenticator *) ), this, SLOT(  authenticationRequired(  const QString , quint16 , QAuthenticator *)  ) );
    QObject::connect( &m_HttpObj, SIGNAL( proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ), this, SLOT(  proxyAuthenticationRequired ( QNetworkProxy , QAuthenticator * ) ) );
    QObject::connect( &m_HttpObj, SIGNAL( readyRead ( QHttpResponseHeader ) ), this, SLOT(  readyRead ( QHttpResponseHeader ) ) );
}

HttpDownload::~HttpDownload()
{
    RSDM_LOG_FUNC;
    disconnect();
    m_HttpObj.disconnect();
    //delete &m_HttpObj;
}
void HttpDownload::start()
{
    initFile();
    setState( DownloadState::DOWNLOADING, true );
    QUrl url(m_UrlAddress.c_str());
	
    m_apHttpRequestHeader->setRequest("GET", url.path() );
    m_apHttpRequestHeader->setValue("Host",  url.host() );
    m_apHttpRequestHeader->setValue("Connection", "Keep-Alive");
    m_apHttpRequestHeader->setValue("User-Agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50");
    m_HttpObj.setHost(  url.host() );
    m_HttpObj.request( *( m_apHttpRequestHeader ) );
    
}

void HttpDownload::stop()
{
    DownloadState::States curState = state();
	if( curState  == DownloadState::STOPPED
       || curState  == DownloadState::DONE
       || curState  == DownloadState::FAILED)
        return ; 
    
    setState( DownloadState::PAUSED, true );
    m_HttpObj.abort();
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
    if( isFalse )
    {
        setError( m_HttpObj.errorString().toStdString());
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
    qint64 bytes = m_HttpObj.bytesAvailable();
    buff = new char[bytes];
    
    calculateProgress( done, total );
    iBytes2 = m_HttpObj.read(buff, bytes);
    if ( -1 == iBytes2)
        qDebug()<<("ERROR while reading from Http stream ");
    else
    {
        if( openFile() )
        {
            qint64 btmp = writeToFile(buff,iBytes2);
            if( -1 == btmp )
            {
                qDebug()<<("write failed");
                return;
            }
        }
    }
    delete[] buff;
    
    if( done == total ) 
    {
        setState(DownloadState::DONE );
    }
}

void HttpDownload::responseHeaderReceived( const QHttpResponseHeader & resp)
{
	
}

void HttpDownload::done(const bool & error)
{
	if( error )
    {
        setError(m_HttpObj.errorString().toStdString());
        return ;
    }
    
    if( state() == DownloadState::DONE )
    {   
        emit statusChanged( state() );
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
