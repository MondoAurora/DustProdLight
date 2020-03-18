/*
 * dpl_impl_meta.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl_meta.h"
#include "dpl_impl.h"

using namespace std;

const DPLEntity DPLImplMeta::UnitModel = DPL_MBI_UNIT_MODEL;
const DPLEntity DPLImplMeta::TypeStore = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Store", DPLImplMeta::UnitModel);
const DPLEntity DPLImplMeta::TypeUnit = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Unit", DPLImplMeta::UnitModel);

const DPLEntity DPLImplMeta::TypeEntity = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Entity", DPLImplMeta::UnitModel);
const DPLEntity DPLImplMeta::AttEntityGlobalId = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "GlobalId", DPLImplMeta::TypeEntity);


const DPLEntity DPLImplMeta::UnitIdea = DPL_MBI_UNIT_IDEA;
const DPLEntity DPLImplMeta::TypeType = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Type", DPLImplMeta::UnitIdea);
const DPLEntity DPLImplMeta::TypeAttribute = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Attribute", DPLImplMeta::UnitIdea);
const DPLEntity DPLImplMeta::TypeReference = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Reference", DPLImplMeta::UnitIdea);

const DPLEntity DPLImplMeta::UnitNarrative = DPL_MBI_UNIT_NARRATIVE;

const DPLEntity DPLImplMeta::ActionSequence = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Sequence", DPLImplMeta::UnitNarrative);
const DPLEntity DPLImplMeta::ActionSelect = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Select", DPLImplMeta::UnitNarrative);
const DPLEntity DPLImplMeta::ActionRepeat = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Repeat", DPLImplMeta::UnitNarrative);

const DPLEntity DPLImplMeta::UnitDialog = DPL_MBI_UNIT_DIALOG;

const DPLEntity DPLImplMeta::ActionDialog = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Dialog", DPLImplMeta::UnitDialog);
const DPLEntity DPLImplMeta::ActionAgent = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Agent", DPLImplMeta::UnitDialog);
const DPLEntity DPLImplMeta::ActionSignal = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Signal", DPLImplMeta::UnitDialog);

const DPLEntity DPLImplMeta::UnitTools = DPL_MBI_UNIT_TOOLS;

const DPLEntity DPLImplMeta::TypeConnected = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Connected", DPLImplMeta::UnitTools);
const DPLEntity DPLImplMeta::RefConnectedOwner = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Owner", DPLImplMeta::TypeConnected);
const DPLEntity DPLImplMeta::RefConnectedExtends = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Extends", DPLImplMeta::TypeConnected);

const DPLEntity DPLImplMeta::TypeDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPLImplMeta::UnitDialog);
const DPLEntity DPLImplMeta::AttDialogActiveAgent = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "ActiveAgentPos", DPLImplMeta::TypeDialog);

const DPLEntity DPLImplMeta::TypeAgent = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLImplMeta::UnitDialog);
const DPLEntity DPLImplMeta::RefAgentRelay = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Relay", DPLImplMeta::TypeAgent);

const DPLEntity DPLImplMeta::TypeCollection = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Collection", DPLImplMeta::UnitTools);
const DPLEntity DPLImplMeta::RefCollectionMembers = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Members", DPLImplMeta::TypeCollection);
const DPLEntity DPLImplMeta::RefCollectionSeparator = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Separator", DPLImplMeta::TypeCollection);

const DPLEntity DPLImplMeta::TypeLink = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Link", DPLImplMeta::UnitTools);
const DPLEntity DPLImplMeta::RefLinkSource = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Source", DPLImplMeta::TypeLink);
const DPLEntity DPLImplMeta::RefLinkTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLImplMeta::TypeLink);


const DPLEntity DPLImplMeta::UnitText = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Text");
const DPLEntity DPLImplMeta::TypeCharacter = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLImplMeta::UnitText);
const DPLEntity DPLImplMeta::AttCharacterChar = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Char", DPLImplMeta::TypeCharacter);

const DPLEntity DPLImplMeta::TypeStream = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Stream", DPLImplMeta::UnitTools);
const DPLEntity DPLImplMeta::AttStreamPos = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Pos", DPLImplMeta::TypeStream);
const DPLEntity DPLImplMeta::AttStreamOK = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "StatusOK", DPLImplMeta::TypeStream);
const DPLEntity DPLImplMeta::AttStreamURL = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "URL", DPLImplMeta::TypeStream);

const DPLEntity DPLImplMeta::ActionDumpChar = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "DumpChar", DPLImplMeta::UnitTools);
const DPLEntity DPLImplMeta::ActionReadStream = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "ReadStream", DPLImplMeta::UnitTools);
