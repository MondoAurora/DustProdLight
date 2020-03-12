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

#include "dpl_impl.h"

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

extern const DPLEntity UnitTools;

extern const DPLEntity TypeConnected;
extern const DPLEntity RefConnectedOwner;
extern const DPLEntity RefConnectedExtends;

}


#endif /* DPL_IMPL_META_H_ */
