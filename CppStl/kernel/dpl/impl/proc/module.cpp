#include "../dpl_impl_meta.h"
#include "module.h"

using namespace DPLImplMeta;

class DustProdLightProcModule: public DPLModule {
public:
	virtual ~DustProdLightProcModule() {

	}

	virtual void* createLogic(int logicId) const {
		//	if (ActionSequence == logicId) {
		//		return new DPLUProcCtxJson();
		//	} else if ( ActionSelect == logicId) {
		//		return new DPLUActionStreamReader();
		//	}	else if ( ActionRepeat == logicId ) {
		//		return new DPLUActionDump();
		//	}

		return NULL;
	}

	virtual void releaseLogic(int logicId, void* pLogic) const {
		//	if (ActionSequence == logicId) {
		//		delete (DPLUProcCtxJson*) pLogic;
		//	} else if ( ActionSelect == logicId) {
		//		delete (DPLUActionStreamReader*) pLogic;
		//	}	else if ( ActionRepeat == logicId ) {
		//		delete (DPLUActionDump*) pLogic;
		//	}
	}

	virtual void init() const {
		DPLMain::registerLogicProvider(this, ActionSequence, ActionSelect, ActionRepeat, DPL_ENTITY_INVALID);
	}
	virtual void release() const {

	}
};

const DPLModule* DPLModuleProc = new DustProdLightProcModule();

