#include "signalreceiverfactory.h"
#include "signalreceiver.h"
#include <QString>
#include <QObject>
#include <downloadmanager.h>
#include <proxy.h>
SignalReceiver* SignalReceiverFactory::create( int mask) 
{
	boost::shared_ptr<SignalReceiver> spRet( new SignalReceiver()) ; 
    
    return spRet.get() ; 
}
