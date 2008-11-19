#include "downloadmanager.h"
#include "idownload.h"
#include "engines/rapidshare/rapidshareengine.h"
#include "enginemanager.h"

#include <boost/bind.hpp>

#include <QTimer>

DownloadManager::DownloadManager() : m_iMaxDownloadFiles(3),m_iCurrentDownloadingFiles(0),m_pEngineManager(new EngineManager())
{
    QTimer::singleShot(1000,this,SLOT(init()));
}
DownloadManager::~DownloadManager()
{
}
void DownloadManager::init()
{
    ;
}
bool handle_tmpr ( const std::pair<std::string, boost::shared_ptr<DownloadEngine> > & en, const std::string & patt ) 
{
    return (en.second)->handleThisPattern(patt);
}
void DownloadManager::addDownload(const std::string & urlAddress, const std::string & destination)
{
    IDownload *pDownload = NULL ; 

    if ( find(urlAddress ) != NULL ) 
        return ; // already in download 
    
    const DownloadEngine *pEngine = m_pEngineManager->findEngineWithPattern(urlAddress);
    if ( !pEngine)
        return ; 
    pDownload = pEngine->spawn() ; 
    pDownload->setUrlAddress(urlAddress) ;
    pDownload->setDestinationAddress(destination);
    m_DownloadList.push_back(IDownloadSmartPtr(pDownload));

    QTimer::singleShot(1000,this,SLOT(slot_listChanged()));
	//FIXME: temporasr
	//pDownload->start()
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
const EngineManager *   DownloadManager::engineManager() const
{
    return m_pEngineManager.get();
};
const std::string & name_hlpr(const DownloadManager::IDownloadSmartPtr & pDownload ) 
{
    return pDownload->urlAddress(); 
}
    
IDownload* DownloadManager::find(const std::string & urlAddress ) 
{
    DownloadListType::iterator it = std::find_if ( m_DownloadList.begin(),m_DownloadList.end(), boost::bind(name_hlpr,_1) == urlAddress ) ;
    if ( it == m_DownloadList.end() )
        return NULL ; 
    return it->get();
}
