#ifndef __PROXY_H__
#define __PROXY_H__

#include <string>
#include <boost/shared_ptr.hpp>

class DownloadManager ; 
class EngineManager ; 
class Settings;
class Proxy 
{
    public:
        ~Proxy();
        /**
         * @brief Will return Download Manager 
         */
       static DownloadManager * downloadManager() ; 
        /**
         * @brief Will return Engine Manager 
         */
		static const EngineManager * engineManager() ; 
        /**
         * @brief Will return Global Settings
         */
		static Settings * settings();
        static void init();
        
        static void setDownloadManager( DownloadManager *ptr ) ; 
        
        
        
	private:
		static Proxy* proxy() ;
		Proxy() ; 
		Proxy(const Proxy & rhs) ; // hidden 
        DownloadManager*                        m_pDownloadManager ;  
        boost::shared_ptr<Settings>		        m_apSettings ; 
};
#endif //__PROXY_H__