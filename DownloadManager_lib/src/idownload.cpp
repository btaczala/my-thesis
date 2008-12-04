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

IDownload::IDownload(OptionsContainer* options) : m_pDownloadInfo( new DownloadState ), m_Options( options ), m_SecondsDownloading(0), m_Error("Success")
{
    m_pDownloadInfo->m_Percentage = 0;
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

unsigned int IDownload::GetProgress() const 
 {
     if( m_pDownloadInfo->m_Percentage != 100 )
     {
 	    if ( m_pDownloadInfo->m_BytesDownloaded != 0 && m_pDownloadInfo->m_DownloadFileSize != 0)
 		{
 			m_pDownloadInfo->m_Percentage = (double)((double)m_pDownloadInfo->m_BytesDownloaded  / (double)m_pDownloadInfo->m_DownloadFileSize  ) * 100 ; 
 		}
     }
//      LOG( QString("GetProgress() returns %1").arg( m_pDownloadInfo->m_Percentage ) );
     return m_pDownloadInfo->m_Percentage ;
 }

void IDownload::setFileName()
{
    QString tmp(m_UrlAddress.c_str());
    tmp = tmp.right(tmp.length() -tmp.lastIndexOf("/") - 1);
    m_FileName = tmp.toStdString();
}

void IDownload::setState(const DownloadState::States& _state, bool triggerEmit)
{
     m_pDownloadInfo->m_State = _state;
     emit( statusChanged(_state) );
}

