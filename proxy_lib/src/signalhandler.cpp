//
// C++ Implementation: signalhandler
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "signalhandler.h"

SignalHandler::SignalHandler()
 : QObject()
{
}


SignalHandler::~SignalHandler()
{
}

void SignalHandler::addReceiver(SignalReceiver * receiver)
{
    m_ListOfReceivers.push_back( SignalReceiverShPtr( receiver ) ) ; 
}

void SignalHandler::setSignalSender(const QObject * sender)
{
    connect ( sender, SIGNAL(), this, SLOT() ) ;
}


