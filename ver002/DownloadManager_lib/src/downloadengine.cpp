//
// C++ Implementation: downloadengine
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <algorithm>
#include "downloadengine.h"

DownloadEngine::DownloadEngine(const std::string & engineName ) : m_EngineName(engineName)
{
}
DownloadEngine::~DownloadEngine()
{

}
const DownloadEngine::StringList& DownloadEngine::patterns() const
{
    return m_UrlPatterns; 
}
void DownloadEngine::setPatterns(const StringList &patt)
{
    m_UrlPatterns = patt ; 
    //std::copy(patt.begin(),patt.end(),m_UrlPatterns.begin());
}
const std::string & DownloadEngine::name() const
{
    return m_EngineName ; 
}
bool DownloadEngine::handleThisPattern( const std::string & pattern ) 
{
    StringList::iterator it = m_UrlPatterns.begin() ; 
    StringList::iterator itEnd = m_UrlPatterns.end() ; 
    for ( ; it != itEnd ; ++it ) 
    {
        if ( pattern.find(*it) != std::string::npos ) 
            return true ; 
    }
    return false ; 
}
void DownloadEngine::setOptionsForEngine(const OptionsContainer& options)
{ 
    m_Options.reset( new OptionsContainer( options ));
}