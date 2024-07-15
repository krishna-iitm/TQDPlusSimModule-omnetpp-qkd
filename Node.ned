//
// 
//This code is made by Md Shahbaz Akhtar, Date: 10-08-2021
//Updated by Krishnakumar year 2023
// 
//
// 
//


module Node
{
    parameters:
        int address;
        int n;
        int linkCapacity;
        string appType;
        int TotalnumberOfSlot;
        int Traffic_Class;
        double Slot_Duration;
        double lambda;
        int source_node;
        int destination_node;
        double packetSize;
        //double trustProbability; //Node trust probability
        @display("i=old/x_blank,green,10");
    gates:
        inout port[]; //Edges of the actual graph
    submodules:
        //      app: <appType> like IApp {
        app: App {
            parameters:
                address = address;
                n = n;
                packetSize = packetSize;
                TotalnumberOfSlot = TotalnumberOfSlot;
                Traffic_Class = Traffic_Class;
                timeSlot = Slot_Duration;
                lambda = lambda;
                source_node = source_node;
                destination_node = destination_node;
                //  packetLength = packetLength;
                //           numOfNodes = numOfNodes;
                @display("p=140,60");
        }

        routing: Routing {
            parameters:
                @display("p=140,130");
                TotalnumberOfSlot = TotalnumberOfSlot;
                timeSlot = Slot_Duration;
            gates:
                in[sizeof(port)]; //Routing module has gates to connect to the queueModule; the size is =to #queues, which is =to #edges connected to the node
                out[sizeof(port)]; // This is to transfer packets internally from Routing module to L2Queue Module (Physical Queue) 
        }
        queue[sizeof(port)]: L2Queue { //number of  L2queue modules maintained is =to the number of edges(ports of the node). 
             
            //L2Queue connected to port[e] of node u will resemble the L2Queue connected to port[e] of node v where e=(u,v).
            //Each L2Queue Module consists of 2 cQueue objects, one for X_e and other for Y_e. 
            //Therefore, for implementation, we have 2m L2Queues of which those connected to same edge(port) will evolve identically.  
            parameters:
                TotalnumberOfSlot = TotalnumberOfSlot;
                linkCapacity = linkCapacity;
                timeSlot = Slot_Duration;
                @display("p=80,200,row");
        }
    connections:
        routing.localOut --> app.in;
        routing.localIn <-- app.out;
        for i=0..sizeof(port)-1 //for every edge connected to the node, connect the following
        { 
            routing.out[i] --> queue[i].in;
            routing.in[i] <-- queue[i].out;
            queue[i].line <--> port[i]; //Port is the edges of the node
        }
}


