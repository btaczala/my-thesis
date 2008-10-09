#include "enginemanager.h"
#include "downloadengine.h"
#ifdef ENGINES_BUILT_IN
#include "engines/rapidshare/rapidshareengine.h"
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
    boost::shared_ptr<DownloadEngine> pRSEngine ( new RapidshareEngine() ) ; 
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

const DownloadEngine * EngineManager::findEngineWithPattern(const std::string & UrlPattern ) 
{
    EngineMap::iterator it = std::find_if ( m_Engines.begin(),m_Engines.end(), boost::bind(ifEngineHandlePattern,_1,UrlPattern) == true ) ; 
    if( it == m_Engines.end() ) 
        return NULL ; 
    return it->second.get();
};
