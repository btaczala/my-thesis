#include "enginemanager.h"
#include "downloadengine.h"
#include "engine.h"

#ifdef ENGINES_BUILT_IN
#include "engines/rapidshare/qrapidsharedownload.h"
#include "engines/http/httpdownload.h"
#include "engines/ftp/ftpdownload.h"
#endif
#include <boost/bind.hpp>
/////////////////////////////////////////////////
const std::string & name_hlpr(const std::pair<std::string,boost::shared_ptr<DownloadEngine> > & pair )
{
    return pair.second->name() ; 
}
bool ifEngineHandlePattern(const std::pair<std::string,boost::shared_ptr<DownloadEngine> > & pair, const std::string & pattern ) 
{
    return pair.second->handleThisPattern(pattern);
}
////////////////////////////////////////////////

EngineManager::EngineManager()
{
    loadEngines();
}
void EngineManager::setEnginesDirPath(const std::string &path)
{
    m_EnginesDirPath = path;
}
void EngineManager::loadEngines(const std::string &path)
{
    if ( !path.empty() ) 
        setEnginesDirPath(path);
#ifdef ENGINES_BUILT_IN
    /*
    pierwsze dwa :
    * http
    * ftp
    ...

    * rapidshare
    * megaupload
    * porntube

    */
    //boost::shared_ptr<DownloadEngine> pRSEngine ( new RapidshareEngine() ) ;
    DownloadEngine::StringList slHttp;
    slHttp.push_back("http://");
    
    boost::shared_ptr<DownloadEngine> pEngineHttp ( new Engine<HttpDownload>("http"));
    pEngineHttp->setPatterns(slHttp);
    m_Engines[pEngineHttp->name()] = pEngineHttp;
    
    DownloadEngine::StringList slFtp;
    slFtp.push_back("ftp://");
    
    boost::shared_ptr<DownloadEngine> pEngineFtp ( new Engine<FtpDownload>("ftp"));
    pEngineFtp->setPatterns(slFtp);
    m_Engines[pEngineFtp->name()] = pEngineFtp;
    
    
    DownloadEngine::StringList sl;
    sl.push_back("http://rapidshare");
    sl.push_back("http://rs");
    //sl.push_back("rapids");

    boost::shared_ptr<DownloadEngine> pRSEngine ( new Engine<QRapidshareDownload>("rapidshare"));
    pRSEngine->setPatterns(sl);
    m_Engines[pRSEngine->name()] = pRSEngine;
#endif 
    
    /*
    loadLibrary () and so on 
    */
}

DownloadEngine * EngineManager::findEngine(const std::string & engineName ) const 
{
    EngineMap::iterator it = std::find_if ( m_Engines.begin(),m_Engines.end(), boost::bind(name_hlpr,_1) == engineName ) ; 
    if ( it == m_Engines.end() ) 
        return NULL ; 
    return it->second.get() ; 
};

const DownloadEngine * EngineManager::findEngineWithPattern(const std::string & UrlPattern ) // http://rapidshare.com/files/kupa.rar
{
    // protocols 
    // http
    // ftp
    /*

    */
    EngineMap::iterator it = std::find_if ( m_Engines.begin(),m_Engines.end(), boost::bind(ifEngineHandlePattern,_1,UrlPattern) == true ) ; 
    if( it == m_Engines.end() ) 
        return NULL ; 
    
    if ( it->second.get()->name() == "http" )
        // szukaj innego, od 
    {
        EngineMap::iterator tmp = it;
        EngineMap::iterator it2 = std::find_if( ++it,m_Engines.end(), boost::bind(ifEngineHandlePattern,_1,UrlPattern) == true );
        if ( it2 == m_Engines.end() ) 
            return tmp->second.get();
        return it2->second.get();
    }

    return it->second.get();
};

const EngineManager::EngineMap & EngineManager::engines() const
{
    return m_Engines;
}
