//
// C++ Interface: downloadengine
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DOWNLOADENGINE_H
#define DOWNLOADENGINE_H

#include <string>
#include <list>
#include <map>
class IDownload ; 
/**
	@author 
*/

class DownloadEngine{
    public:
        typedef                 std::list<std::string>  StringList ; 
                                DownloadEngine(const std::string & engineName );
        virtual                 ~DownloadEngine();
        virtual bool            handleThisPattern( const std::string & pattern ) = 0 ; 
        virtual IDownload *     spawn() ; 
        virtual void            setOptionsForEngine( const std::map<std::string, void*> & options ) = 0 ; 
        void                    setPatterns(const StringList & patt ) ;
        const StringList &      patterns() const ; 
        const std::string       name() const ; 
    protected:
        StringList              m_UrlPatterns ; 
    private:
        std::string             m_EngineName ; 

};

#endif
