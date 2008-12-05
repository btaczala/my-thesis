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

#include "idownload.h"
#include "rslogger.h"
QString DownloadStateToString( DownloadState::States state ) 
{
    switch ( state ) 
    {
        
    case DownloadState::INIT :
        return QObject::tr("Initialization");
        break ; 
    case DownloadState::STOPPED:
        return QObject::tr("Stopped");
        break ; 
    case DownloadState::DOWNLOADING:
        return QObject::tr("Downloading");
        break ; 
    case DownloadState::PAUSED:
        return QObject::tr("Paused");
        break ; 
    case DownloadState::DONE:
        return QObject::tr("Finished");
        break ; 
    case DownloadState::FAILED:
        return QObject::tr("Failed");
        break ; 
    default:
        return "";
    };
}

IDownload::IDownload(OptionsContainer* options) 
: m_pDownloadInfo( new DownloadState )
, m_Options( options )
, m_SecondsDownloading(0)
, m_Error("Success")
{
}

IDownload::~IDownload()
{
    RSDM_LOG_FUNC;
};

void IDownload::setUrlAddress(const std::string & urlAddrr)
{
    m_UrlAddress = urlAddrr ; 
    setFileName();
}
const std::string & IDownload::urlAddress() const
{
    return m_UrlAddress ; 
}
void IDownload::setDestinationAddress(const std::string &localAddress)
{
    m_FileDestination = localAddress ; 
}
const std::string & IDownload::destinationAddress() const
{
    return m_FileDestination ; 
}

unsigned int IDownload::progress() const 
 {
    if ( m_pDownloadInfo->m_DownloadedBytes != 0 && m_pDownloadInfo->m_TotalBytes != 0)
    {
        return (double)((double)m_pDownloadInfo->m_DownloadedBytes  / (double)m_pDownloadInfo->m_TotalBytes  ) * 100 ; 
    }
    return 0;
 }

void IDownload::setFileName()
{
    QString tmp(m_UrlAddress.c_str());
    tmp = tmp.right(tmp.length() -tmp.lastIndexOf("/") - 1);
    m_FileName = tmp.toStdString();
}

void IDownload::setState(const DownloadState::States& _state, bool triggerEmit)
{
    if( m_pDownloadInfo->m_State != DownloadState::DOWNLOADING && _state == DownloadState::DOWNLOADING )
    {
        startTimer(Download::TIMERINT);
    }
    
    if( m_pDownloadInfo->m_State == DownloadState::DOWNLOADING && _state != DownloadState::DOWNLOADING )
    {
        killTimer(m_TimerId);
        m_SecondsDownloading = 0;
    }
     m_pDownloadInfo->m_State = _state;
     emit( statusChanged(_state) );
}

void IDownload::setError( const std::string& _err )
{
     m_Error = _err; 
     qDebug() <<_err.c_str();
}

void IDownload::timerEvent(QTimerEvent* event)
{
    //set property and specify which events should be sent and which not
    static qint64 prevDownloaded = 0;
    if( m_pDownloadInfo->m_DownloadedBytes - prevDownloaded > 0 )
    {
        emit downloadRate( QString("%1").arg( ((double) (m_pDownloadInfo->m_DownloadedBytes - prevDownloaded)/ 1024),0, 'f',2) );
    }   
    m_SecondsDownloading++;
    emit( elapsedTime(m_SecondsDownloading / 2));
    emit( bytesRead( m_pDownloadInfo->m_DownloadedBytes, m_pDownloadInfo->m_TotalBytes ));
    prevDownloaded = m_pDownloadInfo->m_DownloadedBytes;
    
}
/*
        //fixme: calculateProgress()
        m_pDownloadInfo->m_DownloadFileSize = total ; 
        int bytesDownloadedOverall = (m_pDownloadInfo->m_DownloadFileSize - total) > 0 ?  m_pDownloadInfo->m_DownloadFileSize - total : 0 ; 
        m_pDownloadInfo->m_BytesDownloaded = done + bytesDownloadedOverall; 
        double dDone = m_pDownloadInfo->m_BytesDownloaded;
        double dTotal = m_pDownloadInfo->m_DownloadFileSize;
        double dResTotal = dDone / dTotal;
        dResTotal *= 100;
        m_Progress = (int)dResTotal;
//         qDebug() << "emit :DownloadStatus("<< m_Progress <<")";  
        if ( m_emitCounter == scSkipEmit ) 
        {
            emit bytesRead(dDone,dTotal) ;
            m_emitCounter = 0 ; 
        }
        else 
            m_emitCounter++;
        m_pDownloadInfo->bytesReadPreviously =m_pDownloadInfo->bytesReadCurrent;
        m_pDownloadInfo->bytesReadCurrent = done ; 
        */


