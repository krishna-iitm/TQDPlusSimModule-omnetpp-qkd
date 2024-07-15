//
//
//

//#include <stdio.h>
//#include <string.h>
//#include <omnetpp.h>
#include "L2Queue.h"
#include "networkTopoInfo.h"
using namespace omnetpp;

/**
 * Point-to-point interface module. While one frame is transmitted,
 * additional frames get queued up; see NED file for more info.
 */


Define_Module(L2Queue);

L2Queue::L2Queue()
{
    endTransmissionEvent = nullptr;
    timeSlotMsg = nullptr;
    targetModuleNetworkManager=nullptr;
//    eventForVirtualQueueUpdate = nullptr;
}

L2Queue::~L2Queue()
{
    cancelAndDelete(endTransmissionEvent);
    cancelAndDelete(timeSlotMsg);
   // cancelAndDelete(eventForVirtualQueueUpdate);

}

void L2Queue::initialize()
{
    targetModuleNetworkManager = getModuleByPath("RandomGraph.networkController");
    myAddress = getParentModule()->par("address");
    QueueCapacity = par("QueueCapacity");
//    EV<<"Queue capacity is: "<<QueueCapacity<<endl;
   // eventForVirtualQueueUpdate = new cMessage("virtualQueueUpdating");
    timeToUpdateVirtualQueue = 1;  // in second
 //   virtualQueueLength=1;
    myAddress = getParentModule()->par("address");
    minLinkWeight = 1;
 //   WATCH(virtualQueueLength);
    timeSlot =  par("timeSlot");
    TotaltimeSlot = par("TotalnumberOfSlot");
    timeSlotCounter=0;
//    queue.setName("queue");
    queueForUncryptedPackets.setName("queueForUncryptedPackets");
    queueForEncryptedPackets.setName("queueForEncryptedPackets");

  //  endTransmissionEvent = new cMessage("endTxEvent");

    if (par("useCutThroughSwitching"))
        gate("line$i")->setDeliverOnReceptionStart(true);
//        myAddress = getParentModule()->par("address");

 //   frameCapacity = par("frameCapacity");
    linkCapacity = par("linkCapacity");

/*Registering signals used in NED here to record statistics*/
       xQlenSignal = registerSignal("xQlenSignal");
       yQlenSignal = registerSignal("yQlenSignal");
       qlenSignal = registerSignal("qlen");

//    busySignal = registerSignal("busy");
 //     queueingTimeSignal = registerSignal("queueingTime");
       dropSignal = registerSignal("drop");
//    txBytesSignal = registerSignal("txBytes");
//    rxBytesSignal = registerSignal("rxBytes");
       emit(xQlenSignal, queueForUncryptedPackets.getLength());
       emit(yQlenSignal, queueForEncryptedPackets.getLength());
//    emit(busySignal, false);
       isBusy = false;
 //   cGate *gate = gate("line");
  //  cModule
 //   EV<<" is the gate is connected outside queue "<<gate("line$o")->getNextGate()->isConnected()<<endl;
    if (gate("line$o")->getNextGate()->isConnected())
    {
    //   EV<<"Link capacity is: "<< linkCapacity<<endl;
       timeSlotMsg = new cMessage("self msg for next time slot in queue");
       scheduleAt(timeSlot, timeSlotMsg);
    }

}

void L2Queue::startTransmitting(cMessage *msg)
{
    EV << "Starting transmission of " << msg << endl;
    isBusy = true;
    int64_t numBytes = check_and_cast<cPacket *>(msg)->getByteLength();
    send(msg, "line$o");

    simtime_t endTransmission = gate("line$o")->getTransmissionChannel()->getTransmissionFinishTime();
    scheduleAt(endTransmission, endTransmissionEvent);

}

void L2Queue::handleMessage(cMessage *msg)
{

    if (strcmp(msg->getName(),"BroadCast Packet")==0)
    {
        Packet *pk = check_and_cast<Packet *>(msg);
        if (msg->arrivedOn("line$i"))
        {
            send(msg, "out");
        }
        else
            {
      //          EV<<"Received the unencrypted broadcast packet to physical queue X generated in time slot "<<pk->getTimeSlotCounter()<<endl;
                pk->setTimestamp();
                queueForUncryptedPackets.insert(msg);

            }

    }

    if (strcmp(msg->getName(),"Unicast Packet")==0)
    {
        Packet *pk = check_and_cast<Packet *>(msg);
        if (msg->arrivedOn("line$i"))
        {
            send(msg, "out");
        }

        else if (pk->getEnableEncryption())
            {
               if(queueForUncryptedPackets.getLength()>= QueueCapacity)
               {
                   EV<<"queue is full. Droping the packet!"<<endl;
                   emit(dropSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
                    delete pk;
               }
               else
               {
            EV<<"received packet in physical queue X\n";
        //        EV<<"Received the unencrypted unicast packet to physical queue X generated in time slot "<<pk->getTimeSlotCounter()<<endl;
                pk->setTimestamp();
                queueForUncryptedPackets.insert(msg);
                emit(qlenSignal, queueForUncryptedPackets.getLength());
               }
            }

        else
        {
            EV<<"received packet in physical queue Y directly\n"<<endl;
            pk->setTimestamp();
            queueForEncryptedPackets.insert(pk);
        }


    }


    if(msg == timeSlotMsg)
       {
        networkTopoInfo *targetTopoInfo = check_and_cast<networkTopoInfo*>(targetModuleNetworkManager);

        if (queueForUncryptedPackets.getLength()>0)
            {

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
             keyStorageBank = targetTopoInfo->returnKeyStorageBank();

//             map<int, map<int, int> >::iterator itr6;
            // For accessing inner map
//            map<int, int>::iterator ptr6;
//            EV << "\nKey storage Bank detail in Routing Module before encryption is"<<endl;
//            for (itr6 = keyStorageBank.begin(); itr6 != keyStorageBank.end(); itr6++) {
//
//                 for (ptr6 = itr6->second.begin(); ptr6 != itr6->second.end(); ptr6++) {
//
//                         EV << "Before: Available key between the node " <<itr6->first<< " and "<<ptr6->first<<" is "<< ptr6->second<<endl;
//                 }
//             }
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//            EV<<"\nNumber of packets in Physical Queue X before encryption between edge "<<myAddress<<"--->"<<gate("line$o")->getNextGate()->getIndex()<<" are: "<<queueForUncryptedPackets.getLength()<<endl;
//            EV<<"\nkey available between the edge before encryption "<<myAddress<<"--->"<<gate("line$o")->getNextGate()->getIndex()<<" is "<< keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()]<<endl;

            int tempMinValueBetweenAvailableKeyAndPacketNumber = min(queueForUncryptedPackets.getLength(), keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()]);
            for (int i=0; i<tempMinValueBetweenAvailableKeyAndPacketNumber; i++)
            {
             //  EV<<"\nKey available. Encryption begins!!!!\n";
               cObject *obj = (cMessage *)queueForUncryptedPackets.pop();
               Packet *pk = check_and_cast<Packet *>(obj);
               pk->setTotalQueueDelay_X(pk->getTotalQueueDelay_X() + (simTime() - pk->getTimestamp()));
 //              EV << "Packet is encrypted with the available key and forwarded to physical queue Y !! and remaining key is : " << (keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()] - 1) <<endl;
               pk->setTimestamp();
               queueForEncryptedPackets.insert(pk);
//               EV<<"Received the Encrypted packets from physical queue X to physical queue Y"<<endl;
               keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()] = keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()] - 1;
               keyStorageBank[gate("line$o")->getNextGate()->getIndex()][myAddress] = keyStorageBank[gate("line$o")->getNextGate()->getIndex()][myAddress] - 1;
            }

            if (keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()]==0)
            {
//                EV<<"\nSorry no more keys are there for encryption!!\n";
//                EV<<"\nNumber of packets in Physical Queue X after encryption between edge "<<myAddress<<"--->"<<gate("line$o")->getNextGate()->getIndex()<<" are: "<<queueForUncryptedPackets.getLength()<<endl;
//                EV<<"\nkey available between the edge after encryption "<<myAddress<<"--->"<<gate("line$o")->getNextGate()->getIndex()<<" is "<< keyStorageBank[myAddress][gate("line$o")->getNextGate()->getIndex()]<<endl;
//                EV<<"\nNumber of packets in Physical Queue Y after encryption between edge "<<myAddress<<"--->"<<gate("line$o")->getNextGate()->getIndex()<<" are: "<<queueForEncryptedPackets.getLength()<<endl;

            }

             }

        if (queueForEncryptedPackets.getLength()>0)
            {
         //   EV<<"link capacity is: "<<linkCapacity<<endl;
            int tempMinVaueLinkCapacityAndQueueLength = min(linkCapacity,queueForEncryptedPackets.getLength());
            for(int i=0; i<tempMinVaueLinkCapacityAndQueueLength; i++)
            {
            cObject *obj = (cMessage *)queueForEncryptedPackets.pop();
            Packet *pk = check_and_cast<Packet *>(obj);
            pk->setTotalQueueDelay_Y(pk->getTotalQueueDelay_Y() + (simTime() - pk->getTimestamp()));
     //       EV<<"Sending the encrypted packets generated in time slot: "<< pk->getTimeSlotCounter()<< "from physical queue Y to node: "<<gate("line$o")->getNextGate()->getIndex() <<endl;
            send(pk, "line$o");
            }
            }

        emit(xQlenSignal, queueForUncryptedPackets.getLength());
        emit(yQlenSignal, queueForEncryptedPackets.getLength());
        if (timeSlotCounter<TotaltimeSlot)
          {
            timeSlotCounter++;
            scheduleAt(simTime() + timeSlot, timeSlotMsg);

          }
       }

 }

//
//     if(msg == timeSlotMsg)
//     {
//
//         if(queue.getLength()>0)
//         {
//         cObject *obj = (cMessage *)queue.pop();
//         Packet *pk = check_and_cast<Packet *>(obj);
//         EV << "Sending the encrypted packet " << pk->getPacketName() << " to node " <<pk->getRoute(pk->getArraySizeAndIndex()-1)<< endl;
//         send(pk, "line$o");
//         }
//         scheduleAt(simTime() + timeSlot, timeSlotMsg);
//
//     }
//     else if (msg->arrivedOn("line$i"))
//     {
//    send(msg, "out");
//
//     }
//     else
//     {
//         EV<<" Received the encrypted packets from physical queue X to physical queue Y"<<endl;
//         queue.insert(msg);
//     }
//
  // }   till here
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
     //////////////////////////////////////////////
//              if (pk->arrivedOn("in")) {
//                 // Packet *pk = check_and_cast<Packet *>(msg);
//
//                  queue.insert(pk);
//
//                         }
///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
//
//     if (msg == endTransmissionEvent) {
//        // Transmission finished, we can start next one.
//        EV << "Transmission finished.\n";
//        isBusy = false;
//        if (queue.isEmpty()) {
//     //       emit(busySignal, false);
//        }
//        else {
//            msg = (cMessage *)queue.pop();
//            EV<<" queuing time is "<< simTime() - msg->getTimestamp()<<endl;
//   //         emit(queueingTimeSignal, simTime() - msg->getTimestamp());
//   //         emit(qlenSignal, queue.getLength());
//            startTransmitting(msg);
//        }
//    }
//else if (msg->arrivedOn("line$i")) {
//            // pass up
//        //    emit(rxBytesSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
//            send(msg, "out");
//         //   virtualQueueLength++;
//        }
//    else {  // arrived on gate "in"
//        if (endTransmissionEvent->isScheduled()) {
//            // We are currently busy, so just queue up the packet.
//            if (frameCapacity && queue.getLength() >= frameCapacity) {
//                EV << "Received " << msg << " but transmitter busy and queue full: discarding\n";
//  //              emit(dropSignal, (long)check_and_cast<cPacket *>(msg)->getByteLength());
//                delete msg;
//            }
//            else {
//                EV << "Received " << msg << " but transmitter busy: queueing up\n";
//                msg->setTimestamp();
//           //     virtualQueueLength++;
//                queue.insert(msg);
//    //           emit(qlenSignal, queue.getLength());
//            }
//        }
//        else {
//            // We are idle, so we can start transmitting right away.
//            EV << "Received " << msg << endl;
//    //       emit(queueingTimeSignal, SIMTIME_ZERO);
//       //     virtualQueueLength++;
//            startTransmitting(msg);
//         //   emit(busySignal, true);
//        }
//    }


void L2Queue::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 0, isBusy ? "transmitting" : "idle");
    getDisplayString().setTagArg("i", 1, isBusy ? (queue.getLength() >= 3 ? "red" : "yellow") : "");
}

