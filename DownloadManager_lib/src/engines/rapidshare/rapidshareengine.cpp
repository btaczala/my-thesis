#include "rapidshareengine.h"
RapidshareEngine::RapidshareEngine() : DownloadEngine() 
{
    DownloadEngine::StringList sl ; 
    sl.push_back("rapidshare");
    sl.push_back("rs");
    sl.push_back("rapids");
    setPatterns(sl);
};
IDownload * RapidshareEngine::handleThisPattern(const std::string &pattern)
{
    return NULL ; 
}
