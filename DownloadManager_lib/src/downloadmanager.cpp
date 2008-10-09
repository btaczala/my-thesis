#include "downloadmanager.h"
#include "idownload.h"
#include "engines/rapidshare/rapidshareengine.h"

#include <boost/bind.hpp>

#include <QTimer>
DownloadManager::DownloadManager() : m_iMaxDownloadFiles(3),m_iCurrentDownloadingFiles(0)
{
    QTimer::singleShot(1000,this,SLOT(init()));
}
void DownloadManager::init()
{
    ;
}
void DownloadManager::loadEngines()
{
    RapidshareEngine *pRSEngine = new RapidshareEngine() ;
    m_DownloadEngines[pRSEngine->name()] = (boost::shared_ptr<DownloadEngine>(pRSEngine));
}
IDownload* DownloadManager::find(const std::string & pattern)
{
    IDownload* pRet = NULL  ; 
    DownloadListType::iterator it = m_DownloadList.begin() ;
    DownloadListType::iterator itEnd = m_DownloadList.end() ;  
    for ( /*it*/ ; it != itEnd ; ++it ) 
    {
        //it->get()->urlAddress() == pattern ; 
        IDownload *pRet = it->get() ; 
        if ( pRet->urlAddress() == pattern )
            return pRet ; 
    };
    return pRet ; 
}
bool handle_tmpr ( const std::pair<std::string, boost::shared_ptr<DownloadEngine> > & en, const std::string & patt ) 
{
    return (en.second)->handleThisPattern(patt);
}
void DownloadManager::addDownload(const std::string & urlAddress, const std::string & destination)
{
    IDownload *pDownload = NULL ; 
    IDownload *pTmp = NULL ; 

    if ( find(urlAddress ) != NULL ) 
        return ; // already in download 

    EngineMapType::iterator it= std::find_if (m_DownloadEngines.begin(), m_DownloadEngines.end(),boost::bind(handle_tmpr, _1, urlAddress) == true ) ; 
    if ( it == m_DownloadEngines.end() ) 
        return ; 
    pDownload = it->second->spawn();
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

const std::string & temp_fun ( const std::pair<std::string, boost::shared_ptr<DownloadEngine> > & en ) 
{
    return en.second->name() ; 
}
DownloadEngine * DownloadManager::findEngine(const std::string & engineName)
{
    EngineMapType::iterator it = std::find_if ( m_DownloadEngines.begin(), m_DownloadEngines.end(), boost::bind(temp_fun,_1) == engineName ) ; 
    if ( it != m_DownloadEngines.end() ) 
        return (it->second).get() ; 
    else
        return NULL ; 
}
