#ifndef FiCo4OMNeT_LOGICAL_H
#define FiCo4OMNeT_LOGICAL_H

#include "CanDataFrame_m.h"
#include "CanList_m.h"
#include "DataDictionaryDefinition_m.h"
#include "DataDictionaryValue_m.h"
#include "ScheduleMsg_m.h"
#include "omnetpp/cmessage.h"
#include "omnetpp/csimplemodule.h"
#include "omnetpp/simtime_t.h"
#include <map>
#include <vector>

namespace FiCo4OMNeT {
class Logical : public omnetpp::cSimpleModule {
public:
	~Logical() noexcept override;

protected:
	void initialize() override;
	void handleMessage(omnetpp::cMessage* msg) override;

private:
	void parseCANInput();
	void parseCANOutput();
	void parseDDOutput();

	void handleResume();
	void handlePause();

	void requestDataDict();
	void requestCANFrames();

	void writeDataDicts();
	void sendCANFrames();
	void clearReceivedDataDicts();

	void localyStoreReceivedFrame(CanDataFrame* frame);
	void localyStoreReadDataDict(DataDictionaryValue* value);

	omnetpp::cMessage* executionMsg{nullptr};
	omnetpp::cMessage* scheduleMsg{nullptr};
	TaskState          state{TaskState::Blocked};

	omnetpp::simtime_t period{};
	omnetpp::simtime_t executionTime{};
	omnetpp::simtime_t executionTimeLeft{};

	struct CanDefiniton {
		CanDefiniton(const CanDataFrameDefinition* def, int id)
		    : definition(def)
		    , gateId(id) {}

		// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
		const CanDataFrameDefinition* definition;
		int gateId;   // ID of the CAN Bus receiving/transmitting the CANMessage

		// NOLINTEND(misc-non-private-member-variables-in-classes)
	};

	struct DataDictDefinition {
		DataDictDefinition(const DataDictionaryDefinition* def, int id)
		    : definition(def)
		    , gateId(id) {}
		// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
		const DataDictionaryDefinition* definition;
		int                             gateId;   // ID of the gate connecting to the DD module

		// NOLINTEND(misc-non-private-member-variables-in-classes)
	};

	std::vector<CanDefiniton> canInput{};
	std::vector<CanDefiniton> canOutput{};
	std::vector<DataDictDefinition>
	    canBusOutputDicts{};   // Datadicts received from CAN bus that should be outputed
	std::vector<DataDictDefinition>
	    localOutputDicts{};   // Datadicts that are generated by this logical

	std::vector<DataDictionaryValue*>
	    receivedFrameDicts{};                         // Datadicts comming from external physicals
	std::vector<DataDictionaryValue*> localDicts{};   // Datadicts that are local in this physical

	omnetpp::simtime_t minimalDependencyTime{};
};
}   // namespace FiCo4OMNeT
#endif