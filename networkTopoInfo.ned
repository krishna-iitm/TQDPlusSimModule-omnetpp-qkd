//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.


simple networkTopoInfo 
{
   
    parameters:
    double packetLengtNetworkTopo;
    int linkCapacityNetworkTopo;
    double timeSlot;
    int TotalnumberOfSlot;
    double min_key_Gen_rate;
    double max_key_Gen_rate;
    int numOfNodes;
    string trustProbabilities;
    double lagrangeMultiplierV; //constant V: multiplier for the penalty
    double lambda;
    @signal[keyBankStatusWithStorage](type="long");
    @signal[keyBankStatusWithoutStorage](type="long");
    @signal[sumUtilitySignal](type="double");
    //@signal[routeCountSignal]; // No type specified for a string signal
    @statistic[keyBankStatusWithStorage](title="queue length";record=timeavg,max,mean,vector;interpolationmode=sample-hold);
    @statistic[keyBankStatusWithoutStorage](title="queue length";record=timeavg,max,mean,vector;interpolationmode=sample-hold);
    @statistic[sumNetworkUtility](source=sumUtilitySignal; title="sum over time, traffic class over";record=count,last);
    //@statistic[routeCountStatistic](source=routeCountSignal; title="route count"; record=vector);
    // Additional parameters
    gates:
      
}