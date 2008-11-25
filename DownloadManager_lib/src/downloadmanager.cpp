#include "downloadmanager.h"
#include "idownload.h"
#include "engines/rapidshare/rapidshareengine.h"
#include "enginemanager.h"
#include "rslogger.h"

#include <boost/bind.hpp>
#include <QDebug>
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
    LOG("void DownloadManager::init()");
}
void DownloadManager::addDownload(const std::string & urlAddress, const std::string & destination)
{
    LOG(QString("void DownloadManager::addDownload(const std::string & urlAddress = %1, const std::string & destination = %2)").arg(urlAddress.c_str() ).arg(destination.c_str() ));
    
    IDownload *pDownload = NULL ; 

    if ( find(urlAddress ) != NULL ) 
        return ; // already in download 
    
    const DownloadEngine *pEngine = m_pEngineManager->findEngineWithPattern(urlAddress);
    if ( !pEngine)
        return ; 
    pDownload = pEngine->spawn() ; 
    if ( pDownload == NULL ) 
    {
        LOG("Unable to add download");
        return ; 
    }
    pDownload->setUrlAddress(urlAddress) ;
    pDownload->setDestinationAddress(destination);
    connectWith(pDownload);
    m_DownloadList.push_back(IDownloadSmartPtr(pDownload));

    QTimer::singleShot(1000,this,SLOT(slot_listChanged()));
    pDownload->start();
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
void DownloadManager::statusChanged(DownloadState::States what)
{
    qDebug() << (int) what;
    int pos = getPositionWithinSlot( sender() ) ;  
    if ( pos !=-1 )
        emit statusChanged(pos,what);
}

void DownloadManager::downloadDone()
{
    int pos = getPositionWithinSlot( sender() ) ;  
    if ( pos !=-1 )
        emit downloadDoneAt(pos); 
}
void DownloadManager::downloadRate(const QString & dwnlRate)
{
    qDebug() << dwnlRate;
}
int DownloadManager::getPercentage() 
{
    DownloadListType::iterator it = m_DownloadList.begin(); 
    DownloadListType::iterator itEnd = m_DownloadList.end(); 
    int count  = m_DownloadList.size(); 
    unsigned int sum = 0 ; 
    unsigned int total = 0;
    for ( it ; it != itEnd ;++it ) 
    {
        sum += (*it)->GetBytesDownloaded();
        total += (*it)->GetFileSize();
    }
    return sum = ((double)sum/(double)total) * 100 ; 
}
void DownloadManager::connectWith(IDownload * pDownload)
{
    QObject::connect ( pDownload, SIGNAL( done() ), this,SLOT( downloadDone() ) ) ;
    QObject::connect ( pDownload, SIGNAL( statusChanged( DownloadState::States ) ), this,SLOT( statusChanged(DownloadState::States) ) ) ;
    QObject::connect ( pDownload, SIGNAL( bytesRead( int , int ) ), this,SLOT( bytesRead( int , int ) ) ) ;
};

int DownloadManager::findPosition(const std::string & url)
{
    int pos = 0 ; 
    for ( int z = 0 ; z < m_DownloadList.size() ; ++z ) 
    {
        if ( m_DownloadList[z]->urlAddress() == url ) 
            return z ; 
    }
    return -1;
}

int DownloadManager::getPositionWithinSlot( QObject * sender )
{
    IDownload *pDownload = qobject_cast<IDownload*>( sender ); 
    return findPosition( pDownload->urlAddress() ); 
}

void DownloadManager::bytesRead(int read, int total)
{
    int pos = getPositionWithinSlot( sender() ) ;  
    if ( pos !=-1 )
        emit bytesReadAt(pos,read,total);
    
}

