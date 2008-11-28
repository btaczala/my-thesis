#ifndef __PROXY_H__
#define __PROXY_H__

#include <string>
#include <memory>

class DownloadManager ; 
class EngineManager ; 
class Settings;
class Proxy 
{
    public:
        /**
         * @brief Will return Download Manager 
         */
		static DownloadManager * downloadManager() ; 
		static const EngineManager * engineManager() ; 
		static Settings * settings();
	private:
		static Proxy* proxy() ;
		Proxy() ; 
		Proxy(const Proxy & rhs) ; // hidden 
		std::auto_ptr<DownloadManager>		m_apDownloadManager ; 
        std::auto_ptr<Settings>		        m_apSettings ; 
};
#endif //__PROXY_H__