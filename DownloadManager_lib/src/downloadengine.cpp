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

DownloadEngine::DownloadEngine(const std::string & patt ) : m_UrlPattern ( patt )  
{
}


DownloadEngine::~DownloadEngine()
{
}

const std::string & DownloadEngine::pattern() const
{
    return m_UrlPattern; 
}


