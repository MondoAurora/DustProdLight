
#include "json.h"

using namespace DPLUnitJson;

class DPLModuleImpl: public DPLModule {
public:
	virtual ~DPLModuleImpl() {

	}

	virtual void init() {
	}

	virtual DPLNarrativeLogic* createLogic(int logicId) const {
		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLNarrativeLogic* pLogic) const {
	}

	virtual void release() {
	}
};

DPLModule* DPLModuleJson::Module = new DPLModuleImpl();

