#include "dpl_module.h"
#include "dpl_proc.h"

using namespace DPLMetaDPLMain;

class DustProdLightProcModule: public DPLModule {
public:
	virtual ~DustProdLightProcModule() {

	}

	virtual void* createLogic(int logicId) const {
			if (ActionSequence == logicId) {
				return new ProcActionSequence();
			} else if ( ActionSelect == logicId) {
				return new ProcActionSelect();
			}	else if ( ActionRepeat == logicId ) {
				return new ProcActionRepeat();

			}	else if ( ActionSignal == logicId ) {
				return new ProcActionSignal();

			}	else if ( ActionAgent == logicId ) {
				return new DustProdLightAgent();
			}	else if ( ActionDialog == logicId ) {
				return new DustProdLightDialog();

			}	else if ( ActionReadStream == logicId ) {
				return new DPLUActionStreamReader();
			}	else if ( ActionDumpChar == logicId ) {
				return new DPLUActionDump();
			}

		return NULL;
	}

	virtual void releaseLogic(int logicId, void* pLogic) const {
			if (ActionSequence == logicId) {
				delete (ProcActionSequence*) pLogic;
			} else if ( ActionSelect == logicId) {
				delete (ProcActionSelect*) pLogic;
			} else if ( ActionRepeat == logicId) {
				delete (ProcActionRepeat*) pLogic;

			} else if ( ActionSignal == logicId) {
				delete (ProcActionSignal*) pLogic;

			} else if ( ActionAgent == logicId) {
				delete (DustProdLightAgent*) pLogic;
			} else if ( ActionDialog == logicId) {
				delete (DustProdLightDialog*) pLogic;

			} else if ( ActionReadStream == logicId) {
				delete (DPLUActionStreamReader*) pLogic;
			}	else if ( ActionDumpChar == logicId ) {
				delete (DPLUActionDump*) pLogic;
			}
	}

	virtual void init() const {
		DustProdLightRuntime::init();

		DPLMain::registerLogicProvider(this,
				ActionSequence, ActionSelect, ActionRepeat, ActionSignal,
				ActionAgent, ActionDialog,
				ActionReadStream, ActionDumpChar,
				DPL_ENTITY_INVALID);
	}
	virtual void release() const {
		DustProdLightRuntime::release();
	}
};

const DPLModule* DPLModuleDPLMain = new DustProdLightProcModule();

