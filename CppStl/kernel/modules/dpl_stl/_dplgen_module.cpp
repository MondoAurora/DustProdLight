#include <_dplgen_module_dpl_stl.h>
#include "dpl_stl.h"

using namespace DPLUnitNarrative;
using namespace DPLUnitDialog;
using namespace DPLUnitTools;
using namespace DPLUnitDust;

class DustProdLightProcModule: public DPLModule {
public:
	virtual ~DustProdLightProcModule() {

	}

	virtual void init() {
		DustProdLightRuntime::init();

		DPLData::setRef(ActionDump, DPL_MBI_REF_CONNECTED_EXTENDS, SvcAction);
		DPLData::setRef(ActionReadStream, DPL_MBI_REF_CONNECTED_EXTENDS, SvcAction);

		DPLMain::registerLogicProvider(this,
				ActionCtrlSequence, ActionCtrlSelect, ActionCtrlRepeat,
				ActionExecAtom, ActionExecAgent, ActionExecDialog,
				ActionReadStream, ActionDump,
				DPL_ENTITY_INVALID);
	}

	virtual DPLAction* createLogic(int logicId) const {
			if (ActionCtrlSequence == logicId) {
				return new ProcActionSequence();
			} else if ( ActionCtrlSelect == logicId) {
				return new ProcActionSelect();
			}	else if ( ActionCtrlRepeat == logicId ) {
				return new ProcActionRepeat();

			}	else if ( ActionExecAgent == logicId ) {
				return new DustProdLightAgent();
			}	else if ( ActionExecDialog == logicId ) {
				return new DustProdLightDialogTokenRing();

			}	else if ( ActionReadStream == logicId ) {
				return new DPLUActionStreamReader();
			}	else if ( ActionDump == logicId ) {
				return new DPLUActionDump();
			}

		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLAction* pLogic) const {
			if (ActionCtrlSequence == logicId) {
				delete (ProcActionSequence*) pLogic;
			} else if ( ActionCtrlSelect == logicId) {
				delete (ProcActionSelect*) pLogic;
			} else if ( ActionCtrlRepeat == logicId) {
				delete (ProcActionRepeat*) pLogic;

			} else if ( ActionExecAgent == logicId) {
				delete (DustProdLightAgent*) pLogic;
			} else if ( ActionExecDialog == logicId) {
				delete (DustProdLightDialogTokenRing*) pLogic;

			} else if ( ActionReadStream == logicId) {
				delete (DPLUActionStreamReader*) pLogic;
			}	else if ( ActionDump == logicId ) {
				delete (DPLUActionDump*) pLogic;
			}
	}

	virtual void release() {
		DustProdLightRuntime::release();
	}
};

DPLModule* DPLModuleDplStl::Module = new DustProdLightProcModule();

