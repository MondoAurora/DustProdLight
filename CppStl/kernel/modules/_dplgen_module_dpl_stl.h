#ifndef DPLGEN_MODULE_DPL_STL_H_
#define DPLGEN_MODULE_DPL_STL_H_

#include <dpl.h>

namespace DPLUnitModel {
extern const DPLEntity UnitModel;

extern const DPLEntity TypeStore;
extern const DPLEntity TypeUnit;
extern const DPLEntity TypeEntity;

extern const DPLEntity AttEntityGlobalId;

extern const DPLEntity ValEntityNull;
extern const DPLEntity ValEntityTrue;
extern const DPLEntity ValEntityFalse;
}

namespace DPLUnitIdea {
extern const DPLEntity UnitIdea;

extern const DPLEntity TypeType;
//extern const DPLEntity RefTypeAttributes;
//extern const DPLEntity RefTypeReferences;

extern const DPLEntity TypeAttBool;
extern const DPLEntity TypeAttInt;
extern const DPLEntity TypeAttDouble;
extern const DPLEntity TypeAttString;

extern const DPLEntity TypeRefSingle;
extern const DPLEntity TypeRefSet;
extern const DPLEntity TypeRefArr;
extern const DPLEntity TypeRefMap;

extern const DPLEntity TypeConst;
extern const DPLEntity TypeTag;
extern const DPLEntity TypeAgent;
}

namespace DPLUnitNarrative {
extern const DPLEntity UnitNarrative;

extern const DPLEntity AgentDialog;

extern const DPLEntity AgentSequence;
extern const DPLEntity AgentSelect;
extern const DPLEntity AgentRepeat;
}

namespace DPLUnitDialog {
extern const DPLEntity UnitDialog;

}

namespace DPLUnitNative {
extern const DPLEntity UnitNative;

extern const DPLEntity TypeModule;
extern const DPLEntity RefModuleAgents;
extern const DPLEntity RefModuleAlgorithms;

extern const DPLEntity TypeService;
extern const DPLEntity TypeCommand;

extern const DPLEntity SvcAction;
extern const DPLEntity CmdActionExecute;

extern const DPLEntity SvcResource;
extern const DPLEntity CmdResourceInit;
extern const DPLEntity CmdResourceRelease;

extern const DPLEntity AgentCall;
extern const DPLEntity RefAgentCallTarget;
extern const DPLEntity RefAgentCallCommand;
extern const DPLEntity RefAgentCallParam;

extern const DPLEntity StoreGeneratedSource;
}

namespace DPLUnitDust {
extern const DPLEntity UnitDust;

extern const DPLEntity AgentRuntime;
extern const DPLEntity RefRuntimeMain;

extern const DPLEntity AgentSystem;
extern const DPLEntity AgentCore;
extern const DPLEntity AgentBlock;
}

namespace DPLUnitTools {
extern const DPLEntity UnitTools;

extern const DPLEntity TypeConnected;
extern const DPLEntity RefConnectedOwner;
extern const DPLEntity RefConnectedExtends;

extern const DPLEntity TypeCollection;
extern const DPLEntity RefCollectionMembers;
extern const DPLEntity RefCollectionSeparator;

extern const DPLEntity TypeLink;
extern const DPLEntity RefLinkSource;
extern const DPLEntity RefLinkTarget;

extern const DPLEntity TypeStream;
extern const DPLEntity AttStreamURL;
extern const DPLEntity AttStreamPos;
extern const DPLEntity AttStreamOK;

extern const DPLEntity AgentReadStream;
extern const DPLEntity AgentDump;

extern const DPLEntity TypeLimits;
extern const DPLEntity AttLimitsIntMin;
extern const DPLEntity AttLimitsIntMax;

}

namespace DPLModuleDplStl {
extern DPLModule* Module;
}

#endif /* DPLGEN_MODULE_DPL_STL_H_ */
