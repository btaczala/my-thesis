#ifndef __PROXY_H__
#define __PROXY_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <QString>

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
        static void deinit();
        
        //static void setDownloadManager( DownloadManager *ptr ) ; 
        
        static QString encrypt(QString& data);
        static QString decrypt(QString& data);
        
	private:
		static Proxy* proxy() ;
		Proxy() ; 
		Proxy(const Proxy & rhs) ; // hidden 
        boost::shared_ptr<DownloadManager>      m_pDownloadManager ;  
        boost::shared_ptr<Settings>		        m_apSettings ; 
};
#endif //__PROXY_H__