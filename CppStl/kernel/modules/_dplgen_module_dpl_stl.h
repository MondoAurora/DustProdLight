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
extern const DPLEntity TypeAttribute;
extern const DPLEntity TypeReference;

extern const DPLEntity TypeAgent;
extern const DPLEntity TypeTag;
extern const DPLEntity TypeConstant;
}

namespace DPLUnitNarrative {
extern const DPLEntity UnitNarrative;

extern const DPLEntity TypeDialog;

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
extern const DPLEntity RefModuleActions;
extern const DPLEntity RefModuleAlgorithms;

extern const DPLEntity TypeService;
extern const DPLEntity TypeCommand;

extern const DPLEntity TypeExecAtom;
extern const DPLEntity RefExecAtomTarget;
extern const DPLEntity RefExecAtomCommand;
extern const DPLEntity RefExecAtomParam;

extern const DPLEntity SvcAction;
extern const DPLEntity CmdActionExecute;

extern const DPLEntity SvcResource;
extern const DPLEntity CmdResourceInit;
extern const DPLEntity CmdResourceRelease;
}

namespace DPLUnitDust {
extern const DPLEntity UnitDust;

extern const DPLEntity TypeRuntime;
extern const DPLEntity RefRuntimeMain;

extern const DPLEntity TypeSystem;

extern const DPLEntity TypeCore;

extern const DPLEntity TypeBlock;
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
