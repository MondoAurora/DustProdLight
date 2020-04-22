
#include "json.h"

using namespace DPLUnitJson;

class DPLModuleJsonImpl: public DPLModule {
public:
	virtual ~DPLModuleJsonImpl() {

	}

	virtual void init(DPLEntity eModule) {
		DPLData::setRef(eModule, DPLUnitNative::RefModuleAlgorithms, buildAlgorithmJsonParser(), AgentJsonParser);
	}

	virtual DPLNarrativeLogic* createLogic(int logicId) const {
		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLNarrativeLogic* pLogic) const {
	}

	virtual void release() {
	}
};

DPLModule* DPLModuleJson::Module = new DPLModuleJsonImpl();

