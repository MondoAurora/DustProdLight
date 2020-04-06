/*
 * dpl_impl_meta.h
 *
 * Meta layer for DPL implementation
 * gets boot items out to "normal" constants, add others
 * "pseudo-generated" source
 *
 *  Created on: Mar 06, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_META_H_
#define DPL_IMPL_META_H_

#include <dpl.h>

namespace DPLUnitModel {
extern const DPLEntity UnitModel;

extern const DPLEntity TypeStore;
extern const DPLEntity TypeUnit;
extern const DPLEntity TypeEntity;

extern const DPLEntity AttEntityGlobalId;
extern const DPLEntity RefEntityActions;
}

namespace DPLUnitIdea {
extern const DPLEntity UnitIdea;

extern const DPLEntity TypeType;
extern const DPLEntity TypeAttribute;
extern const DPLEntity TypeReference;
extern const DPLEntity TypeTag;
}

namespace DPLUnitNarrative {

extern const DPLEntity UnitNarrative;

extern const DPLEntity ActionSequence;
extern const DPLEntity ActionSelect;
extern const DPLEntity ActionRepeat;

extern const DPLEntity SvcProcessor;
extern const DPLEntity CmdProcess;
}

namespace DPLUnitDialog {

extern const DPLEntity UnitDialog;

extern const DPLEntity TypeTask;
extern const DPLEntity RefTaskCommand;
extern const DPLEntity RefTaskTarget;

extern const DPLEntity TypeDialog;
extern const DPLEntity AttDialogActiveAgent;

extern const DPLEntity TypeAgent;
extern const DPLEntity RefAgentRelay;

extern const DPLEntity ActionDialogTokenRing;
extern const DPLEntity ActionAgent;
extern const DPLEntity ActionSignal;
}

namespace DPLUnitDust {
extern const DPLEntity UnitDust;
extern const DPLEntity TypeSystem;
extern const DPLEntity TypeModule;
extern const DPLEntity TypeRuntime;
extern const DPLEntity TypeBlock;

extern const DPLEntity RefRuntimeMain;
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
}

namespace DPLUnitText {
extern const DPLEntity UnitText;

extern const DPLEntity TypeCharacter;
extern const DPLEntity AttCharacterChar;

extern const DPLEntity TypeTextPlain;
extern const DPLEntity AttTextString;
}

namespace DPLModuleDplStl {
extern DPLModule* Module;
}

#endif /* DPL_IMPL_META_H_ */
