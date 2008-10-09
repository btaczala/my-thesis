#include "idownload.h"
IDownload::IDownload() : m_pDownloadInfo( new DownloadState ){};
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