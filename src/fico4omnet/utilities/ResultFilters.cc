#include "fico4omnet/utilities/ResultFilters.h"

//Auto-generated messages
#include "fico4omnet/linklayer/can/messages/CanDataFrame_m.h"
#include "fico4omnet/linklayer/can/messages/ErrorFrame_m.h"
#include "fico4omnet/linklayer/flexray/messages/FRFrame_m.h"

namespace FiCo4OMNeT {

Register_ResultFilter("timestampAge", TimestampAgeFilter);

void TimestampAgeFilter::receiveSignal(cResultFilter *prev, simtime_t_cref t,
        cObject *object, cObject *details) {
    (void)prev;
    if (cMessage *msg = dynamic_cast<cMessage *>(object)) {
//        cMessage *msg = dynamic_cast<cMessage *> (object);
        fire(this, t, t - msg->getTimestamp(), details);
    }
    else
    {
        fire(this, t, object, details);
    }
}

Register_ResultFilter("ID", IDFilter);

void IDFilter::receiveSignal(cResultFilter *prev, simtime_t_cref t,
        cObject *object, cObject *details) {
    (void)prev;
#ifdef WITH_CAN_COMMON
    if (CanDataFrame* dataFrame = dynamic_cast<CanDataFrame *>(object)) {
        fire(this, t, static_cast<uintval_t> (dataFrame->getCanID()), details);
        return;
    }
    if (ErrorFrame* errorFrame = dynamic_cast<ErrorFrame *>(object)) {
        fire(this, t, static_cast<uintval_t> (errorFrame->getCanID()), details);
        return;
    }
    if (ErrorFrame* errorFrame = dynamic_cast<ErrorFrame *>(object)) {
        fire(this, t, static_cast<uintval_t> (errorFrame->getCanID()), details);
        return;
    }
#endif

#ifdef WITH_FR_COMMON
    if (FRFrame* frFrame = dynamic_cast<FRFrame *>(object)) {
        fire(this, t, static_cast<uintval_t> (frFrame->getFrameID()), details);
        return;
    }
#endif
    fire(this, t, object, details);
}

Register_ResultFilter("lowHighRatio", LowHighRatioFilter);

LowHighRatioFilter::LowHighRatioFilter() {
    low = 0;
    high = 0;
    last = 0;
    last_time = SimTime(-1);
}

bool LowHighRatioFilter::process(simtime_t& t, double& value, __attribute__((unused)) cObject *details) {
    //no value yet? we start calculating with the first emitted value to prevent wrong calculation in warmup
    if (this->last_time < 0){
        this->last = value;
        this->last_time = t;
        value = 0;
    }
    else{
        if (this->last > 0)
            high += (t - this->last_time);
        else
            low += (t - this->last_time);
        this->last = value;
        this->last_time = t;
        if ((high + low) > 0)
            value = high / (high + low);
        else
            value = 0;
    }
    return true;
}

Register_ResultFilter("rmNaN", RmNaNFilter);

RmNaNFilter::RmNaNFilter()
{
    this->hadValues = false;
}

bool RmNaNFilter::process(simtime_t& t, double& value, __attribute__((unused)) cObject *details)
{
    (void)t;
    (void)value;
    if(!this->hadValues){
        this->hadValues = true;
    }
    return true;
}

void RmNaNFilter::finish(cResultFilter * prev)
{
    (void)prev;
    if(!this->hadValues){
        fire(this, simTime(), static_cast<uintval_t> (0), nullptr);
    }
}

}
