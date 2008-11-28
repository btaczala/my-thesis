//
// C++ Interface: enginemanager
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ENGINEMANAGER_H
#define ENGINEMANAGER_H

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
class DownloadEngine;
class EngineManager
{
public:
    typedef std::map<std::string,boost::shared_ptr<DownloadEngine> > EngineMap ; 
                            EngineManager();
    void                    setEnginesDirPath(const std::string & path ) ; 
    const std::string &     enginesDirPath() const ; 
    void                    loadEngines(const std::string & path = "" ) ; 
    
    DownloadEngine *        findEngine(const std::string & engineName ) const; 
    const DownloadEngine *  findEngineWithPattern(const std::string & UrlPattern);
    
    const EngineMap &       engines() const ; 
private:
    mutable EngineMap       m_Engines ; 
    std::string             m_EnginesDirPath ; 
    void                    addEngine() ; 
};
#endif //ENGINEMANAGER_H
