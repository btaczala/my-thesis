// #include "signalreceiverfactory.h"
// #include "signalreceiver.h"
// #include <QString>
// #include <QObject>
// #include <downloadmanager.h>
// #include <proxy.h>
// SignalReceiver* SignalReceiverFactory::create( int mask )
// {
// 	boost::shared_ptr<SignalReceiver> spRet( new SignalReceiver()) ; 
//     spRet->m_bits = ( SignalReceiver::Bits ) mask ; 
//     if ( mask == SignalReceiver::NONE ) 
//         return spRet.get() ; 
//     if ( mask == SignalReceiver::ALL ) 
//     {
//         QObject::connect( Proxy::downloadManager() ,
//                           SIGNAL(statusChanged ( int , DownloadState::States ) ), 
//                           spRet.get(), 
//                           SLOT(statusChanged_slot ( int , DownloadState::States ) )
//                         );
//         QObject::connect( Proxy::downloadManager() ,
//                           SIGNAL(statusChanged ( int , DownloadState::States ) ), 
//                           spRet.get(), 
//                           SLOT(SR_statusChanged ( int , DownloadState::States ) )
//                         ); 
//     } 
//     return spRet.get() ; 
// }
