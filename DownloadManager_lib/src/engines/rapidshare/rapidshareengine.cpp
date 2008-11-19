#include "rapidshareengine.h"
#include <map>

RapidshareEngine::RapidshareEngine() : DownloadEngine("rapidshare"), m_UserName(""),m_UserPass("")
{
    DownloadEngine::StringList sl ; 
    sl.push_back("rapidshare");
    sl.push_back("rs");
    sl.push_back("rapids");
    setPatterns(sl);
};
void RapidshareEngine::setOptionsForEngine(const std::map< std::string, void * > &options)
{
    std::map< std::string, void * >::const_iterator it = options.find(RS_USER_NAME);
    if ( it != options.end() ) 
    {
        ;
    }
};
IDownload* RapidshareEngine::spawn() const 
{
	//FIXME: create QRapidshareDownload 
    return NULL ; 
}