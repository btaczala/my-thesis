#include "proxy.h"


#include <downloadmanager.h>
#include <enginemanager.h>
#include <optionscontainer.h>
#include <downloadengine.h>
#include <rslogger.h>
#include <QTime>
#include <cmath>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include "settings.h"

namespace
{
    static const QString _appUid = "E246EC21-B7BD-4c2b-B0B2-A8A157D9AE16";

    struct advancedXor13
    {
        QChar operator()(QChar& c)
        {
            return c.unicode() ^ 13;
        }
    };
}

Proxy * Proxy::proxy() 
{
	static std::auto_ptr<Proxy> pr ( new Proxy() ) ; 
	return pr.get() ;
}
Proxy::Proxy() : m_apSettings( new Settings() ) , m_pDownloadManager( new DownloadManager() ) 
{
};
Proxy::~Proxy()
{
    RSDM_LOG_FUNC;
}

DownloadManager * Proxy::downloadManager() 
{
    if ( proxy()->m_pDownloadManager== NULL ) 
        LOG(QString("Download Manager ptr is NULL " ) );
	return proxy()->m_pDownloadManager.get(); 
}
const EngineManager * Proxy::engineManager() 
{
	return proxy()->m_pDownloadManager->engineManager() ; 
}
Settings * Proxy::settings()
{
   return proxy()->m_apSettings.get() ; 
};
void Proxy::init()
{
    proxy()->m_apSettings->loadSettings();
    proxy()->m_pDownloadManager->init();
}

void Proxy::deinit()
{
    proxy()->m_apSettings.reset();
    proxy()->m_pDownloadManager.reset();
}

QString Proxy::encrypt(const QString& data)
{
    QString buf(data);
    //haha - we're using advanced Xor 13 algorithm
    std::transform(buf.begin(), buf.end(), buf.begin(), advancedXor13());
    return buf;
}

int foo(const int a)
{
    return a-1;
}
QString Proxy::decrypt(const QString& data)
{
    QString buf(data);
    //using namespace boost::lambda;    
    //std::transform(buf.begin(),buf.end(),buf.begin(), boost::lambda::_1 = ( boost::bind( &QChar::unicode, QChar('z') ) ) );
    //std::for_each( buf.begin(),buf.end(),boost::bind( &QChar::unicode, boost::lambda::_1  ) );
    std::transform(buf.begin(), buf.end(), buf.begin(), advancedXor13());
//    int a = boost::bind(foo,1);
    return buf;
}
QString Proxy::getAppUid()
{
    return _appUid;
}
