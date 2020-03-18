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

#include "../dpl.h"

namespace DPLImplMeta {
extern const DPLEntity UnitModel;

extern const DPLEntity TypeStore;
extern const DPLEntity TypeUnit;
extern const DPLEntity TypeEntity;
extern const DPLEntity AttEntityGlobalId;

extern const DPLEntity UnitIdea;

extern const DPLEntity TypeType;
extern const DPLEntity TypeAttribute;
extern const DPLEntity TypeReference;

extern const DPLEntity UnitNarrative;

extern const DPLEntity ActionSequence;
extern const DPLEntity ActionSelect;
extern const DPLEntity ActionRepeat;

extern const DPLEntity UnitDialog;

extern const DPLEntity TypeDialog;
extern const DPLEntity AttDialogActiveAgent;

extern const DPLEntity TypeAgent;
extern const DPLEntity RefAgentRelay;

extern const DPLEntity ActionDialog;
extern const DPLEntity ActionAgent;
extern const DPLEntity ActionSignal;

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
extern const DPLEntity ActionDumpChar;


extern const DPLEntity UnitText;

extern const DPLEntity TypeCharacter;
extern const DPLEntity AttCharacterChar;

}


#endif /* DPL_IMPL_META_H_ */
