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
	connect ( Proxy::downloadManager(), SIGNAL( downloadAdded( int ) ), instance.get(), SLOT( downloadAdded( int ) ) ) ;
	connect ( Proxy::downloadManager(), SIGNAL( downloadRemoved( int ) ), instance.get(), SLOT( downloadRemoved( int ) ) ) ;
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
void SignalPlayGround::downloadAdded( int position ) 
{
	Q_FOREACH( SignalPlayGround::BitsOfListenerPair item, m_MapOfListeners ) 
    {
//         if ( item.first == ALL ) 
            item.second->downloadAdded( position );
    }
}
void SignalPlayGround::downloadRemoved( int position ) 
{
	Q_FOREACH( SignalPlayGround::BitsOfListenerPair item, m_MapOfListeners ) 
    {
//         if ( item.first == ALL ) 
            item.second->downloadRemoved( position );
    }
}
void SignalPlayGround::disconnectFromPlayGround(const QString & name)
{
    SignalPlayGround * instance = SignalPlayGround::instance() ; 
    instance->m_MapOfListeners.remove(name) ; 
}
