#include "downloadmanager.h"
#include "downloadengine.h"
#include "idownload.h"
#include "enginemanager.h"
#include "rslogger.h"

#include <boost/bind.hpp>

#include <QDebug>
#include <QTimer>
#include <QMutexLocker>

DownloadManager::DownloadManager() : m_iMaxDownloadFiles(3),m_iCurrentDownloadingFiles(0),m_pEngineManager(new EngineManager()), m_State ( STOPPED )
{
    //QTimer::singleShot(1000,this,SLOT(init()));
    init();
}
DownloadManager::~DownloadManager()
{
    DownloadListType::iterator itEnd = m_DownloadList.end() ;
    for ( DownloadListType::iterator it = m_DownloadList.begin() ; it != itEnd ; ++it ) 
    {
        (*it)->stop();
    }
}
void DownloadManager::init()
{
    LOG("void DownloadManager::init()");
    m_DownloadManagerSettings.m_CurrentDownloadingFiles = 0 ;
    m_DownloadManagerSettings.m_MaxDownloadingFiles = 3 ;
    m_DownloadManagerSettings.m_CurrentDownloadingFiles = 0 ;
    setState ( DOWNLOADING ) ; 
    
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

    //QTimer::singleShot(1000,this,SLOT(slot_listChanged()));
    update() ; 
    //pDownload->start();
};
void DownloadManager::startDownload(const std::string &urlAddress)
{
    LOG(QString("void DownloadManager::startDownload(const std::string &urlAddress=%1)").arg(urlAddress.c_str() ) );
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
    LOG(QString("IDownload* DownloadManager::find(const std::string & urlAddress=%1) ").arg(urlAddress.c_str() ) );
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
int DownloadManager::percentage() 
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
    //QObject::connect ( pDownload, SIGNAL( done() ), this,SLOT( downloadDone() ) ) ;
    QObject::connect ( pDownload, SIGNAL( statusChanged( DownloadState::States ) ), this,SLOT( statusChanged(DownloadState::States) ) ) ;
    QObject::connect ( pDownload, SIGNAL( bytesRead( int , int ) ), this,SLOT( bytesRead( int , int ) ) ) ;
};
int DownloadManager::findPosition(const std::string & url)
{
    int pos = 0 ; 
    for ( unsigned int z = 0 ; z < m_DownloadList.size() ; ++z ) 
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
void DownloadManager::update()
{

    QMutexLocker localMutex(&m_Mutex);
    if ( m_State == DOWNLOADING ) 
    {
        DownloadListType::iterator it = m_DownloadList.begin() ; 
        DownloadListType::iterator itEnd = m_DownloadList.end() ; 
        IDownload *pDownload = NULL ;//it->get() ; 
        int counter = 0 ; 
        for ( it ; it!=itEnd ; ++it ) 
        {
            if ( m_DownloadManagerSettings.m_CurrentDownloadingFiles >=m_DownloadManagerSettings.m_MaxDownloadingFiles ) 
                break ; 
            pDownload = it->get() ; 
            DownloadState::States state = pDownload->state();
            if ( state == DownloadState::DOWNLOADING || state == DownloadState::DONE || state == DownloadState::FAILED ) 
                continue ; 
            if ( canIDownload() ) 
            {
                pDownload->start() ; 
                m_DownloadManagerSettings.m_LastStartedFilePosition = counter ; 
                m_DownloadManagerSettings.m_CurrentDownloadingFiles += 1;
            };
            counter++;
        }
    }
    else
    {
        // stop ( pause ) all downloads
        ;
    }
}

void DownloadManager::setState(DownloadManagerState state)
{
    if ( state == m_State ) 
        return ; 
    m_State = state ; 
    update() ; 
};

bool DownloadManager::canIDownload() const
{
    if ( state() == DOWNLOADING ) 
    {
        if ( m_DownloadManagerSettings.m_CurrentDownloadingFiles < m_DownloadManagerSettings.m_MaxDownloadingFiles ) 
            return true ; 
    }
    return false ; 
};
DownloadManager::DownloadManagerState DownloadManager::state() const
{
    return m_State ; 
}
IDownload * DownloadManager::downloadAt(unsigned int position)
{
    if ( position >= m_DownloadList.size() || position < 0 ) 
    {
        LOG(QString("Unable to get Download. List size=%1, position requested=%2").arg(m_DownloadList.size()).arg(position) );
        return NULL ; 
    };
    return m_DownloadList[position].get() ;
};