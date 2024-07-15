//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// Represents a packet in the network.
//
packet Packet
{
    int srcAddr @packetData;
    int destAddr @packetData;
    int hopCount @packetData;
    double size @packetData;
    int route[];
    int arraySizeAndIndex = 1; //Keeps track of the packet traversal. 
    //For example if the route is 1->6->4->5 and the packet is in node 6 then arraySizeAndIndex contains the position of the next node 4 (third in the route array) 
    simtime_t TotalQueueDelay_X;
    simtime_t TotalQueueDelay_Y;
    string packetName;
    int timeSlotCounter;
    bool enableEncryption;
}

