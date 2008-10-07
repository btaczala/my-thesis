#include "downloadmanager.h"
#include "idownload.h"
#include "engines/rapidshare/rapidshareengine.h"

IDownload* DownloadManager::find(const std::string & pattern)
{
    IDownload* pRet = NULL  ; 
    DownloadListType::iterator it = m_DownloadList.begin() ;
    DownloadListType::iterator itEnd = m_DownloadList.end() ;  
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
    IDownload *pDownload = NULL ; 
    IDownload *pTmp = NULL ; 

    if ( find(urlAddress ) != NULL ) 
        return ; // already in download 
    EngineListType::iterator it = m_apDownloadEngines.begin() ; 
    EngineListType::iterator itEnd = m_apDownloadEngines.end() ; 
    for ( it ; it != itEnd ; ++it )
    {
        DownloadEngine * ptr = it->get() ; 
        RapidshareEngine * ptr2 = (RapidshareEngine *)ptr ; 
        ptr2->handleThisPattern("");
        //pTmp = it->get()->handleThisPattern(urlAddress) ; 
        if ( pTmp != NULL ) 
            pDownload = pTmp ; 
    }
};
