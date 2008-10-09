#include "rapidshareengine.h"

RapidshareEngine::RapidshareEngine() : DownloadEngine("rapidshare"), m_UserName(""),m_UserPass("")
{
    DownloadEngine::StringList sl ; 
    sl.push_back("rapidshare");
    sl.push_back("rs");
    sl.push_back("rapids");
    setPatterns(sl);
};
bool RapidshareEngine::handleThisPattern(const std::string &pattern)
{
//    return std::find_if ( m_UrlPatterns.begin(),m_UrlPatterns.end(),pattern ) != m_UrlPatterns.end() ; 
}
void RapidshareEngine::setOptionsForEngine(const std::map< std::string, void * > &options)
{
    std::map< std::string, void * >::const_iterator it = options.find(RS_USER_NAME);
    if ( it != options.end() ) 
    {
        ;
    } 
    /*
    std::map<char,int> a ; 
    a['z'] ; 
    */
}
