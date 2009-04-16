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
    if(m_apFile.get())
        m_apFile->close();
};

void IDownload::setUrlAddress(const std::string & urlAddrr)
{
    m_UrlAddress = urlAddrr ; 
}
const std::string & IDownload::urlAddress() const
{
    return m_UrlAddress ; 
}
void IDownload::setDestinationAddress(const std::string &localAddress)
{
    m_FileDestination = localAddress;
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

void IDownload::setState(const DownloadState::States& _state, bool triggerEmit)
{

    LOG(DownloadStateToString( _state ));

    switch ( _state )
    {
        case DownloadState::INIT:
            //m_TimerId = startTimer(Download::TIMERINT);
            break;
        case DownloadState::DOWNLOADING:
            m_TimerId = startTimer(Download::TIMERINT);
            break;
        case DownloadState::PAUSED:
            killTimer(m_TimerId);
            break;
        case DownloadState::DONE:
            renameFile();
            timerEvent(NULL);
            break;
        case DownloadState::FAILED:
            killTimer(m_TimerId);
            m_SecondsDownloading = 0;
            break;
        default:
            break;
    }

     m_pDownloadInfo->m_State = _state;

    if( triggerEmit == true )
        emit( statusChanged(_state) );
}

void IDownload::setError( const std::string& _err )
{
     m_Error = _err; 
     FATAL( _err.c_str() );
}

void IDownload::timerEvent(QTimerEvent* event)
{
    ProgressInfo info;
    //static qint64 prevDownloaded = 0;
    if( m_pDownloadInfo->m_DownloadedBytes - m_pDownloadInfo->m_PrevDownloadedBytes > 0 )
    {
        //FIXME : '2' hardcoded
        info._DownloadRate =  QString("%1").arg( ((double) (m_pDownloadInfo->m_DownloadedBytes - m_pDownloadInfo->m_PrevDownloadedBytes)*2/ 1024),0, 'f',2);
    }
    m_SecondsDownloading++;
    info._ElapsedTime = m_SecondsDownloading ;
    info._DownloadedBytes = m_pDownloadInfo->m_DownloadedBytes;
    info._TotalBytes = m_pDownloadInfo->m_TotalBytes;
    LOG("emit( progressInfo())");
    emit( progressInfo( info ));
    m_pDownloadInfo->m_PrevDownloadedBytes = m_pDownloadInfo->m_DownloadedBytes;
    if( info._DownloadedBytes == info._TotalBytes && info._TotalBytes != 0 )
       killTimer(m_TimerId);
}

void IDownload::initFile()
{
    LOG("initFile");
    if( m_FileDestination.empty() || m_UrlAddress.empty() )
    {
        CRITICAL( m_FileDestination.c_str());
        CRITICAL( m_UrlAddress.c_str());
        return;
    }
    else
    {
        m_apFile.reset( new QFile());
        QString fileName(m_FileDestination.c_str());
        if( fileName.right(1) != "/" )
            fileName += "/";

        QString tmp(m_UrlAddress.c_str());
        tmp = tmp.right(tmp.length() - tmp.lastIndexOf("/") - 1);
        
        m_FileName = tmp.toStdString();
        LOG( QString::fromStdString(m_FileName) );
        
        fileName += tmp;
        fileName += Download::TMPSTRING;
        m_apFile->setFileName( fileName );
        LOG( m_apFile->fileName());
    }
}

void IDownload::removeFromFile( const QString& _post )
{
    if( m_apFile.get())
    {
        QString fileName = m_apFile->fileName();
        int ind = fileName.lastIndexOf( _post );
        if ( ind != -1 )
        {
            fileName = fileName.left( ind );
            fileName += Download::TMPSTRING;
            m_apFile->setFileName(fileName);
            qDebug() << m_apFile->fileName();
        }
    }
}

void IDownload::renameFile()
{
    QString dest = m_apFile->fileName();
    QString destTemp = dest ; //
    int ind = dest.lastIndexOf( Download::TMPSTRING );
    dest = dest.left( ind );
    qDebug() << dest;
    QFile::rename( destTemp, dest );
    if ( QFile::exists(destTemp) ) 
        QFile::remove(destTemp);
}

void IDownload::closeFile()
{
	if( m_apFile.get())
    	m_apFile->close();
}

bool IDownload::openFile()
{
    if( m_apFile.get() != NULL )
    {
        if( !m_apFile->isOpen())
        {
            if( !m_apFile->open(QIODevice::WriteOnly | QIODevice::Append) )
            {
                QString note = QString("Unable to open file %1").arg(m_apFile->fileName());
                setError(note.toStdString());
                return false;
            }
        }
       // else
         //   qDebug() << QString("File already opened %1").arg(m_apFile->fileName());
    }
    else
    {
        setError("use initFile() first");
        return false;
    }
    return true;
}

qint64 IDownload::writeToFile( const char* _data, qint64 _amount )
{
    Q_ASSERT( _data );
    Q_ASSERT( m_apFile.get());
    return m_apFile->write( _data, _amount );
}
