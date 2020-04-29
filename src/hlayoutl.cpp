
#include "hlayoutl.h"

const string KStartSlotName = "Start";
const string KEndSlotName = "End";
const string KSlotPrevCpName = "Prev";
const string KSlotNextCpName = "Next";
const string KSlotType = "/*/Modules/ContainerModL/FHLayoutLSlot";

AHLayoutL::AHLayoutL(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainerL(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AHLayoutL::PEType()
{
    return AVContainerL::PEType() + GUri::KParentSep + Type();
}

MUnit* AHLayoutL::GetLastSlot()
{
    MUnit* res = nullptr;
    MUnit* host = GetMan();
    MUnit* end = host->GetNode("./" + KEndSlotName);
    if (end) {
	MVert* endv = end->GetObj(endv);
	if (endv) {
	    MVert* lastv = endv->GetPair(0);
	    if (lastv) {
		MUnit* prevu = lastv->GetObj(prevu);
		if (prevu) {
		    res = prevu->GetMan();
		}
	    }
	}
    }
    return res;
}

MUnit* AHLayoutL::AppendSlot(MUnit* aSlot)
{
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    MUnit* lastSlot = GetLastSlot();
    MUnit* end = host->GetNode("./" + KEndSlotName);
    MUnit* start = host->GetNode("./" + KStartSlotName);
    __ASSERT(start && end);
    string endUri = end->GetUri(this, ETrue);
    string newSlotUri = aSlot->GetUri(this, ETrue);
    string newSlotNextUri = newSlotUri + "/" + KSlotNextCpName;
    string newSlotPrevUri = newSlotUri + "/" + KSlotPrevCpName;
    TNs ns; MutCtx mctx(NULL, ns);
    if (lastSlot) {
	// There are slots already
	// Disconnect last slot from end
	string lastSlotUri = lastSlot->GetUri(this, ETrue);
	string lastSlotPrevUri = lastSlotUri + "/" + KSlotPrevCpName;
	hoste->AppendMutation(TMut(ENt_Disconn, ENa_P, lastSlotPrevUri, ENa_Q, endUri));
	hoste->Mutate(true, false, false, mctx);
	// Connect new slot to last slot
	hoste->AppendMutation(TMut(ENt_Conn, ENa_P, lastSlotPrevUri, ENa_Q, newSlotNextUri));
	hoste->Mutate(true, false, false, mctx);
    } else {
	// There are no slots yet
	// Connect new slot to start
	string startUri = start->GetUri(this, ETrue);
	hoste->AppendMutation(TMut(ENt_Conn, ENa_P, startUri, ENa_Q, newSlotNextUri));
	hoste->Mutate(true, false, false, mctx);
    }
    // Connect new slot to end
    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, newSlotPrevUri, ENa_Q, endUri));
    hoste->Mutate(true, false, false, mctx);
}

string AHLayoutL::GetSlotType()
{
    return KSlotType;
}
