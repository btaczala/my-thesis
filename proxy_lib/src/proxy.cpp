#include "proxy.h"


#include <downloadmanager.h>
#include <enginemanager.h>
#include <optionscontainer.h>
#include <downloadengine.h>

#include "settings.h"

Proxy * Proxy::proxy() 
{
	static std::auto_ptr<Proxy> pr ( new Proxy() ) ; 
	return pr.get() ;
}
/*
void apply_settings( const std::pair<std::string,boost::shared_ptr<DownloadEngine> > & pair ) 
{
    pair.second->setOptionsForEngine( Settings::optionsForEngine(pair.first) ); 
}
*/
Proxy::Proxy() : m_apDownloadManager( new DownloadManager() ), m_apSettings( new Settings() ) 
{
    // should be 
    // std::for_each ( m_apDownloadManager->engineManager()->begin(), m_apDownloadManager->engineManager()->end(), apply_settings( it->name() ) 
    DownloadEngine * pEngine = m_apDownloadManager->engineManager()->findEngine("rapidshare");
    OptionsContainer options ;//= new OptionsContainer();
    options.addOption("username", std::string("4625386"));
    options.addOption("password", std::string("maggot666_rs"));
    pEngine->setOptionsForEngine( options ); 
    
}
DownloadManager * Proxy::downloadManager()
{
	return proxy()->m_apDownloadManager.get() ; 
}
const EngineManager * Proxy::engineManager() 
{
	return proxy()->m_apDownloadManager->engineManager() ; 
}
Settings * Proxy::settings()
{
    return proxy()->m_apSettings.get() ; 
};