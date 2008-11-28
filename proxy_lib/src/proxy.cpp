#include "proxy.h"


#include <downloadmanager.h>
#include <enginemanager.h>
#include <optionscontainer.h>
#include <downloadengine.h>

Proxy * Proxy::proxy() 
{
	static std::auto_ptr<Proxy> pr ( new Proxy() ) ; 
	return pr.get() ;
}
Proxy::Proxy() : m_apDownloadManager( new DownloadManager() ) 
{
    DownloadEngine * pEngine = m_apDownloadManager->engineManager()->findEngine("rapidshare");
    OptionsContainer options ;//= new OptionsContainer();
    options.addOption(scRS_USER_NAME, std::string("4625386"));
    options.addOption(scRS_USER_PASS, std::string("maggot666_rs"));
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