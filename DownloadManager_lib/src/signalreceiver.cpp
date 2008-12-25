#include "signalreceiver.h"
#include <proxy.h>


void SignalPlayGround::connectToPlayGround(ISignalListener * listener, const QString & name, Bits mask )
{
    SignalPlayGround * instance = SignalPlayGround::instance() ; 
    instance->m_MapOfListeners[name] = qMakePair(mask,listener); 
}
SignalPlayGround * SignalPlayGround::instance()
{
    static std::auto_ptr<SignalPlayGround> instance ( new SignalPlayGround() ); 
    connect ( Proxy::downloadManager(), SIGNAL( statusChanged( int , DownloadState::States ) ), instance.get(), SLOT(statusChanged( int , DownloadState::States ) ) ) ;
    connect ( Proxy::downloadManager(), SIGNAL( progressInfoAt(int , const ProgressInfo & ) ), instance.get(), SLOT( progressInfoAt(int , const ProgressInfo & ) ) ) ;
    return instance.get() ; 
}

SignalPlayGround::~ SignalPlayGround()
{
    m_MapOfListeners.clear();
}

void SignalPlayGround::statusChanged(int at, DownloadState::States state)
{
    Q_FOREACH( SignalPlayGround::BitsOfListenerPair item, m_MapOfListeners ) 
    {
//         if ( item.first == ALL ) 
            item.second->statusChanged(at,state);
    }
}

void SignalPlayGround::progressInfoAt(int at, const ProgressInfo & _info)
{
    Q_FOREACH( SignalPlayGround::BitsOfListenerPair item, m_MapOfListeners ) 
    {
//         if ( item.first == ALL ) 
            item.second->progressInfoAt(at,_info);
    }
}

void SignalPlayGround::disconnectFromPlayGround(const QString & name)
{
    SignalPlayGround * instance = SignalPlayGround::instance() ; 
    instance->m_MapOfListeners.remove(name) ; 
}
