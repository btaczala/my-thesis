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
    std::copy(patt.begin(),patt.end(),m_UrlPatterns.begin());
}


