//
// C++ Interface: signalhandler
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <QObject>
#include <vector>

/**
	@author bartek taczała< b@kontrasty.szczecin.pl >
*/
class SignalReceiver ; 
class SignalHandler : public QObject
{
    Q_OBJECT
public:
    typedef boost::shared_ptr<SignalReceiver> SignalReceiverShPtr;
    SignalHandler();
    ~SignalHandler();
    void addReceiver ( SignalReceiver *receiver ) ; 
    void setSignalSender( const QObject * sender ) ; 
private:
    std::vector<SignalReceiverShPtr> m_ListOfReceivers ; 
private slots:
        
};

#endif
