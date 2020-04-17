#ifndef DPLGEN_MODULE_DPL_STL_H_
#define DPLGEN_MODULE_DPL_STL_H_

#include <dpl.h>

namespace DPLUnitModel {
extern const DPLEntity UnitModel;

extern const DPLEntity TypeStore;
extern const DPLEntity TypeUnit;
extern const DPLEntity TypeEntity;

extern const DPLEntity AttEntityGlobalId;
extern const DPLEntity RefEntityActions;

extern const DPLEntity ValEntityNull;
extern const DPLEntity ValEntityTrue;
extern const DPLEntity ValEntityFalse;
}

namespace DPLUnitIdea {
extern const DPLEntity UnitIdea;

extern const DPLEntity TypeType;
extern const DPLEntity RefTypeDefaultActions;

extern const DPLEntity TypeConstant;
extern const DPLEntity TypeAttribute;
extern const DPLEntity TypeReference;
extern const DPLEntity TypeTag;

extern const DPLEntity TypeService;
extern const DPLEntity TypeCommand;
}

namespace DPLUnitNarrative {
extern const DPLEntity UnitNarrative;

extern const DPLEntity TypeAction;
extern const DPLEntity TypeAlgorithm;

extern const DPLEntity TypeCtrlSequence;
extern const DPLEntity TypeCtrlSelect;
extern const DPLEntity TypeCtrlRepeat;

extern const DPLEntity TypeExecAtom;
extern const DPLEntity RefExecAtomCommand;
extern const DPLEntity RefExecAtomTarget;
extern const DPLEntity RefExecAtomParam;

extern const DPLEntity TypeExecAgent;

extern const DPLEntity TypeExecDialog;

extern const DPLEntity TypeExecScheduleTokenRing;

extern const DPLEntity SvcAction;
extern const DPLEntity CmdActionExecute;

extern const DPLEntity SvcResource;
extern const DPLEntity CmdResourceInit;
extern const DPLEntity CmdResourceRelease;
}

namespace DPLUnitDialog {
extern const DPLEntity UnitDialog;

extern const DPLEntity TypeDialog;
}

namespace DPLUnitDust {
extern const DPLEntity UnitDust;

extern const DPLEntity TypeSystem;
extern const DPLEntity TypeModule;
extern const DPLEntity TypeRuntime;

extern const DPLEntity TypeCore;
extern const DPLEntity TypeAgent;
extern const DPLEntity TypeBlock;

extern const DPLEntity RefRuntimeMain;

extern const DPLEntity ActionCtrlSequence;
extern const DPLEntity ActionCtrlSelect;
extern const DPLEntity ActionCtrlRepeat;

//extern const DPLEntity ActionExecAtom;
//extern const DPLEntity ActionExecAgent;
//extern const DPLEntity ActionExecDialog;


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

extern const DPLEntity ActionReadStream;
extern const DPLEntity ActionDump;

extern const DPLEntity TypeLimits;
extern const DPLEntity AttLimitsIntMin;
extern const DPLEntity AttLimitsIntMax;

}

namespace DPLModuleDplStl {
extern DPLModule* Module;
}

#endif /* DPLGEN_MODULE_DPL_STL_H_ */
