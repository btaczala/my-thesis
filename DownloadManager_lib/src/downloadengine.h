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
class IDownload ; 
/**
	@author 
*/

class DownloadEngine{
    public:
        typedef                 std::list<std::string>  StringList ; 
                                DownloadEngine();
        virtual                 ~DownloadEngine();
        virtual IDownload *     handleThisPattern( const std::string & pattern ) = 0 ; 
        void                    setPatterns(const StringList & patt ) ;
        const StringList &      patterns() const ; 


    private:
        StringList              m_UrlPatterns ; 

};

#endif
