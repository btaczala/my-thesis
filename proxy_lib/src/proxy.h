#ifndef __PROXY_H__
#define __PROXY_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <QString>

class DownloadManager ; 
class EngineManager ; 
class Settings;
class SignalHandler ;
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
        /**
         * @brief will return global signal handler
         * @return 
         */
        static const SignalHandler * signalHandler() ; 
        static void init();
        static void deinit();
        
        //static void setDownloadManager( DownloadManager *ptr ) ; 
        
        static QString encrypt(const QString& data);
        static QString decrypt(const QString& data);
        
	private:
		static Proxy* proxy() ;
		Proxy() ; 
		Proxy(const Proxy & rhs) ; // hidden 
        boost::shared_ptr<DownloadManager>      m_pDownloadManager ;  
        boost::shared_ptr<Settings>		        m_apSettings ;
        boost::shared_ptr<SignalHandler>             m_apSignalHandler ;
};
#endif //__PROXY_H__