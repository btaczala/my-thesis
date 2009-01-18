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
#include <memory>

#include "optionscontainer.h"
class IDownload ; 
/**
    @author tdubik@gmail.com
    @author w0lanto@gmail.com
*/

class OptionsContainer;
class DownloadEngine{
    public:
        //FIXME: should be QList<QRegExp> 
        typedef                 std::vector<std::string>  StringList; 
        

                                DownloadEngine(const std::string & engineName );
        virtual                 ~DownloadEngine();
        virtual bool            handleThisPattern( const std::string & pattern ) ; 
        virtual IDownload *     spawn() const = 0; 
        void                    setPatterns(const StringList & patt );
        void                    setOptionsForEngine(const OptionsContainer& options);
        const StringList &      patterns() const ; 
        const std::string &     name() const ;
        int                     percent();
    protected:
        StringList                          m_UrlPatterns ; 
        std::auto_ptr<OptionsContainer>     m_Options;
    private:
        std::string                         m_EngineName ; 

};

#endif
