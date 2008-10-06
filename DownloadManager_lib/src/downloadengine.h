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

class IDownload ; 
/**
	@author 
*/

class DownloadEngine{
    public:
                                DownloadEngine(const std::string & patt = "" );
        virtual                 ~DownloadEngine();
        virtual IDownload *     handleThisPattern( const std::string & pattern ) = 0 ; 
        const std::string &     pattern() const ; 
    private:
        std::string             m_UrlPattern ; 

};

#endif
