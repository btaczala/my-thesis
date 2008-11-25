#include <map>

#include "rapidshareengine.h"
#include "qrapidsharedownload.h"


RapidshareEngine::RapidshareEngine() : DownloadEngine("rapidshare"), m_UserName(""),m_UserPass("")
{
    DownloadEngine::StringList sl ; 
    sl.push_back("rapidshare");
    sl.push_back("rs");
    sl.push_back("rapids");
    setPatterns(sl);
};

void RapidshareEngine::setOptionsForEngine(const std::map< std::string, boost::any > &options)
{
    std::map< std::string, boost::any >::const_iterator it = options.find(scRS_USER_NAME);
    std::string tmp ; 
    if ( it != options.end() ) 
    {
        m_UserName = boost::any_cast<std::string>(it->second);
    }

    it = options.find( scRS_USER_PASS );
    if( it!=options.end() )
    {
        m_UserPass = boost::any_cast<std::string>(it->second);
    }
};
IDownload* RapidshareEngine::spawn() const 
{
    if (m_UserName.empty() || m_UserPass.empty() ) 
    {
        LOG(QString("UserName (%1) or password(%2) not set").arg( m_UserName.c_str() ).arg( m_UserPass.c_str() ) );
        return NULL ; 
    }
    QRapidshareDownload* pDownload = new QRapidshareDownload();
    pDownload->setUser( m_UserName.c_str(), m_UserPass.c_str() );
    return (IDownload*) pDownload;
}