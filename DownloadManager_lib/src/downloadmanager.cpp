#include "downloadmanager.h"
#include "idownload.h"

IDownload* DownloadManager::find(const std::string & pattern)
{
    IDownload* pRet = NULL  ; 
    std::vector<boost::shared_ptr<IDownload> >::iterator it = m_apDownloadList->begin() ;
    std::vector<boost::shared_ptr<IDownload> >::iterator itEnd = m_apDownloadList->begin() ;  
    for ( it ; it != itEnd ; ++it ) 
    {
        //it->get()->urlAddress() == pattern ; 
        IDownload *pRet = it->get() ; 
//        if ( pRet->urlAddress() == pattern )
//            return pRet ; 
    };
    return pRet ; 
}

void DownloadManager::addDownload(const std::string & urlAddress, const std::string & destination)
{
    if ( find(urlAddress ) != NULL ) 
        return ; // already in download 
    //m_apDownloadList->push_back( boost::shared_ptr<IDownload>( new 
    // check for plugins and select correct one . 
}