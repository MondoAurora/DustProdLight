#include "dpl_module.h"
#include "dpl_proc.h"

using namespace DPLUnitNarrative;
using namespace DPLUnitDialog;
using namespace DPLUnitTools;

class DustProdLightProcModule: public DPLModule {
public:
	virtual ~DustProdLightProcModule() {

	}

	virtual DPLAction* createLogic(int logicId) const {
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
			}	else if ( ActionDialogTokenRing == logicId ) {
				return new DustProdLightDialogTokenRing();

			}	else if ( ActionReadStream == logicId ) {
				return new DPLUActionStreamReader();
			}	else if ( ActionDump == logicId ) {
				return new DPLUActionDump();
			}

		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLAction* pLogic) const {
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
			} else if ( ActionDialogTokenRing == logicId) {
				delete (DustProdLightDialogTokenRing*) pLogic;

			} else if ( ActionReadStream == logicId) {
				delete (DPLUActionStreamReader*) pLogic;
			}	else if ( ActionDump == logicId ) {
				delete (DPLUActionDump*) pLogic;
			}
	}

	virtual void init() {
		DustProdLightRuntime::init();

		DPLMain::registerLogicProvider(this,
				ActionSequence, ActionSelect, ActionRepeat, ActionSignal,
				ActionAgent, ActionDialogTokenRing,
				ActionReadStream, ActionDump,
				(DPLAction*) DPL_ENTITY_INVALID);
	}
	virtual void release() {
		DustProdLightRuntime::release();
	}
};

DPLModule* DPLModuleDPLMain = new DustProdLightProcModule();

