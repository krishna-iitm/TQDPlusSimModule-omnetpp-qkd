//

//

simple App like IApp
{
    parameters:
        int address;  // local node address
        int n;//  @prompt("Again enter the Number of nodes");
        int packetLength @unit(byte); // length of one message (fixed! no "volatile" modifier)
        double timeSlot;
        double packetSize;
        double lambda;
        int TotalnumberOfSlot;
        int Traffic_Class;
        int source_node  @prompt("Enter the source node (for unicast and broadcast)");
        int destination_node @prompt("Enter the destination node (for unicast)");
        @display("i=block/browser");
        @signal[endToEndDelayForUnEncryptedPckts](type="simtime_t");
        @signal[endToEndDelayForEncryptedPckts](type="simtime_t");
        @signal[totalDelayByQueue_X](type="simtime_t");
        @signal[totalDelayByQueue_Y](type="simtime_t");
        @signal[totalPropagationDelay](type="simtime_t");
        @signal[hopCount](type="long");
    //   @signal[sourceAddress](type="long");
        @statistic[endToEndDelayForUnEncryptedPckts](title="end-to-end delay for the pckts that don't need quantum encryption";unit=s;record=mean,max;interpolationmode=none);
        @statistic[endToEndDelayForEncryptedPckts](title="end-to-end delay for the pckts that need quantum encryption";unit=s;record=mean,max;interpolationmode=none);
        @statistic[totalDelayByQueue_X](title="Total queuing delay due to physical queue X";unit=s;record=mean,max;interpolationmode=none);
        @statistic[totalDelayByQueue_Y](title="Total queuing delay due to physical queue Y";unit=s;record=mean,max;interpolationmode=none);
        @statistic[totalPropagationDelay](title="Total propagation delay";unit=s;record=mean,max;interpolationmode=none);
        @statistic[hopCount](title="hop count of arrived packets";interpolationmode=none;record=mean,max);
     //   @statistic[sourceAddress](title="source address of arrived packets";interpolationmode=none;record=vector?);
    gates:
        input in;
        output out;
}