#include "idownload.h"
IDownload::IDownload() : m_pDownloadInfo( new DownloadState )
{
    m_pDownloadInfo->m_Percentage = 0;
};
IDownload::~IDownload(){};

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
    m_FileDestination = localAddress ; 
}
const std::string & IDownload::destinationAddress() const
{
    return m_FileDestination ; 
}

unsigned int IDownload::GetProgress() const 
 {
 	if ( m_pDownloadInfo->m_Percentage == 0 ) 
 	{
 		if ( m_pDownloadInfo->m_BytesDownloaded != 0 && m_pDownloadInfo->m_DownloadFileSize != 0)
 		{
 			m_pDownloadInfo->m_Percentage = (double)((double)m_pDownloadInfo->m_BytesDownloaded  / (double)m_pDownloadInfo->m_DownloadFileSize  ) * 100  ; 
 		}
 	}
 	return m_pDownloadInfo->m_Percentage;
 }