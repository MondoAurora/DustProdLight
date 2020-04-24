#include <_dplgen_module_dpl_stl.h>
#include "dpl_stl.h"

using namespace DPLUnitNarrative;
using namespace DPLUnitNative;
using namespace DPLUnitDialog;
using namespace DPLUnitTools;
using namespace DPLUnitDust;

class DustProdLightProcModule: public DPLModule {
public:
	virtual ~DustProdLightProcModule() {

	}

	virtual void init(DPLEntity eModule) {
		DustProdLightRuntime::init();

		DPLData::setRef(eModule, RefModuleAgents, AgentSequence);
		DPLData::setRef(eModule, RefModuleAgents, AgentSelect);
		DPLData::setRef(eModule, RefModuleAgents, AgentRepeat);
		DPLData::setRef(eModule, RefModuleAgents, AgentReadStream);
		DPLData::setRef(eModule, RefModuleAgents, AgentDump);
	}

	virtual DPLNarrativeLogic* createLogic(int logicId) const {
			if (AgentSequence == logicId) {
				return new ProcActionSequence();
			} else if ( AgentSelect == logicId) {
				return new ProcActionSelect();
			}	else if ( AgentRepeat == logicId ) {
				return new ProcActionRepeat();
			}	else if ( AgentReadStream == logicId ) {
				return new DPLUActionStreamReader();
			}	else if ( AgentDump == logicId ) {
				return new DPLUActionDump();
			}

		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLNarrativeLogic* pLogic) const {
			if (AgentSequence == logicId) {
				delete (ProcActionSequence*) pLogic;
			} else if ( AgentSelect == logicId) {
				delete (ProcActionSelect*) pLogic;
			} else if ( AgentRepeat == logicId) {
				delete (ProcActionRepeat*) pLogic;
			} else if ( AgentReadStream == logicId) {
				delete (DPLUActionStreamReader*) pLogic;
			}	else if ( AgentDump == logicId ) {
				delete (DPLUActionDump*) pLogic;
			}
	}

	virtual void release() {
		DustProdLightRuntime::release();
	}
};

DPLModule* DPLModuleDplStl::Module = new DustProdLightProcModule();

