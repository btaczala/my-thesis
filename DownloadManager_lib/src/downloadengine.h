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
#include <vector>
#include <map>
#include <boost/any.hpp>
class IDownload ; 
/**
	@author 
*/

class DownloadEngine{
    public:
        //FIXME: should be QList<QRegExp> 
        typedef                 std::vector<std::string>  StringList ; 
                                DownloadEngine(const std::string & engineName );
        virtual                 ~DownloadEngine();
        virtual bool            handleThisPattern( const std::string & pattern ) ; 
        virtual IDownload *     spawn() const = 0; 
        virtual void            setOptionsForEngine( const std::map<std::string, boost::any> & options ) = 0 ; 
        void                    setPatterns(const StringList & patt ) ;
        const StringList &      patterns() const ; 
        const std::string &     name() const ;
        int                     getPercent();
    protected:
        StringList              m_UrlPatterns ; 
    private:
        std::string             m_EngineName ; 

};

#endif
