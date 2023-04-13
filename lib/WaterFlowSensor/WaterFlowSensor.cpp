#include <stdlib.h>
#include "WaterFlowSensor.h"


void calculateFlowData(float T){
    float Frequenz=0;
    Frequenz=1/T;
    FlowData=compareFlowData(Frequenz);
};

float getFlowData(){
    return FlowData;
};

float compareFlowData(float MatchData){
    //5% Error Range => UPPER 1.05 ; LOWER 0.95
    float UpperBound=MatchData*1.05;
    float LowerBound=MatchData*0.95;
    for (int i=0; i<7;i++){
        if((FlowFrequences[i]>=LowerBound)and(FlowFrequences[i]<=UpperBound)or(FlowFrequences[i]==MatchData)){
            return FlowAmounts[i];
        };
    };
    // Returns Default value 0 and catches Error
    FlowSensorError=true;
    return FlowAmounts[0];
};