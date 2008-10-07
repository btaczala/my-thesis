#include "downloadmanager.h"
#include "idownload.h"
#include "engines/rapidshare/rapidshareengine.h"

#include <QTimer>
DownloadManager::DownloadManager() : m_iMaxDownloadFiles(3),m_iCurrentDownloadingFiles(0)
{
}
IDownload* DownloadManager::find(const std::string & pattern)
{
    IDownload* pRet = NULL  ; 
    DownloadListType::iterator it = m_DownloadList.begin() ;
    DownloadListType::iterator itEnd = m_DownloadList.end() ;  
    for ( it ; it != itEnd ; ++it ) 
    {
        //it->get()->urlAddress() == pattern ; 
        IDownload *pRet = it->get() ; 
        if ( pRet->urlAddress() == pattern )
            return pRet ; 
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
        pTmp = ptr->handleThisPattern("");
        if ( pTmp != NULL )
            pDownload = pTmp ; 
    }
    if ( pDownload ) 
    {
        pDownload->setUrlAddress(urlAddress) ;
        pDownload->setDestinationAddress(destination);
        m_DownloadList.push_back(IDownloadSmartPtr(pDownload));

        QTimer::singleShot(1000,this,SLOT(slot_listChanged()));
    };
};
void DownloadManager::startDownload(const std::string &urlAddress)
{
    IDownload *pDownload ; 
    pDownload = find(urlAddress) ; 
    if ( pDownload == NULL ) 
        return ; 
    pDownload->start() ; 
    QTimer::singleShot(1000,this,SLOT(slot_listChanged()));
}
void DownloadManager::slot_listChanged()
{
}
    