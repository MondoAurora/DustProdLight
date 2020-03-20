/*
 * dpl_impl_meta.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_meta.h"
#include "dpl_boot.h"

using namespace std;

const DPLEntity DPLMetaDPLMain::UnitModel = DPL_MBI_UNIT_MODEL;
const DPLEntity DPLMetaDPLMain::TypeStore = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Store", DPLMetaDPLMain::UnitModel);
const DPLEntity DPLMetaDPLMain::TypeUnit = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Unit", DPLMetaDPLMain::UnitModel);

const DPLEntity DPLMetaDPLMain::TypeEntity = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Entity", DPLMetaDPLMain::UnitModel);
const DPLEntity DPLMetaDPLMain::AttEntityGlobalId = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "GlobalId", DPLMetaDPLMain::TypeEntity);


const DPLEntity DPLMetaDPLMain::UnitIdea = DPL_MBI_UNIT_IDEA;
const DPLEntity DPLMetaDPLMain::TypeType = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Type", DPLMetaDPLMain::UnitIdea);
const DPLEntity DPLMetaDPLMain::TypeAttribute = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Attribute", DPLMetaDPLMain::UnitIdea);
const DPLEntity DPLMetaDPLMain::TypeReference = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Reference", DPLMetaDPLMain::UnitIdea);

const DPLEntity DPLMetaDPLMain::UnitNarrative = DPL_MBI_UNIT_NARRATIVE;

const DPLEntity DPLMetaDPLMain::ActionSequence = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Sequence", DPLMetaDPLMain::UnitNarrative);
const DPLEntity DPLMetaDPLMain::ActionSelect = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Select", DPLMetaDPLMain::UnitNarrative);
const DPLEntity DPLMetaDPLMain::ActionRepeat = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Repeat", DPLMetaDPLMain::UnitNarrative);

const DPLEntity DPLMetaDPLMain::UnitDialog = DPL_MBI_UNIT_DIALOG;

const DPLEntity DPLMetaDPLMain::ActionDialog = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Dialog", DPLMetaDPLMain::UnitDialog);
const DPLEntity DPLMetaDPLMain::ActionAgent = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Agent", DPLMetaDPLMain::UnitDialog);
const DPLEntity DPLMetaDPLMain::ActionSignal = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Signal", DPLMetaDPLMain::UnitDialog);

const DPLEntity DPLMetaDPLMain::UnitTools = DPL_MBI_UNIT_TOOLS;

const DPLEntity DPLMetaDPLMain::TypeConnected = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Connected", DPLMetaDPLMain::UnitTools);
const DPLEntity DPLMetaDPLMain::RefConnectedOwner = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Owner", DPLMetaDPLMain::TypeConnected);
const DPLEntity DPLMetaDPLMain::RefConnectedExtends = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Extends", DPLMetaDPLMain::TypeConnected);

const DPLEntity DPLMetaDPLMain::TypeDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPLMetaDPLMain::UnitDialog);
const DPLEntity DPLMetaDPLMain::AttDialogActiveAgent = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "ActiveAgentPos", DPLMetaDPLMain::TypeDialog);

const DPLEntity DPLMetaDPLMain::TypeAgent = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLMetaDPLMain::UnitDialog);
const DPLEntity DPLMetaDPLMain::RefAgentRelay = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Relay", DPLMetaDPLMain::TypeAgent);

const DPLEntity DPLMetaDPLMain::TypeCollection = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Collection", DPLMetaDPLMain::UnitTools);
const DPLEntity DPLMetaDPLMain::RefCollectionMembers = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Members", DPLMetaDPLMain::TypeCollection);
const DPLEntity DPLMetaDPLMain::RefCollectionSeparator = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Separator", DPLMetaDPLMain::TypeCollection);

const DPLEntity DPLMetaDPLMain::TypeLink = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Link", DPLMetaDPLMain::UnitTools);
const DPLEntity DPLMetaDPLMain::RefLinkSource = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Source", DPLMetaDPLMain::TypeLink);
const DPLEntity DPLMetaDPLMain::RefLinkTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLMetaDPLMain::TypeLink);


const DPLEntity DPLMetaDPLMain::UnitText = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Text");
const DPLEntity DPLMetaDPLMain::TypeCharacter = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLMetaDPLMain::UnitText);
const DPLEntity DPLMetaDPLMain::AttCharacterChar = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Char", DPLMetaDPLMain::TypeCharacter);

const DPLEntity DPLMetaDPLMain::TypeStream = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Stream", DPLMetaDPLMain::UnitTools);
const DPLEntity DPLMetaDPLMain::AttStreamPos = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Pos", DPLMetaDPLMain::TypeStream);
const DPLEntity DPLMetaDPLMain::AttStreamOK = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "StatusOK", DPLMetaDPLMain::TypeStream);
const DPLEntity DPLMetaDPLMain::AttStreamURL = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "URL", DPLMetaDPLMain::TypeStream);

const DPLEntity DPLMetaDPLMain::ActionDumpChar = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "DumpChar", DPLMetaDPLMain::UnitTools);
const DPLEntity DPLMetaDPLMain::ActionReadStream = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "ReadStream", DPLMetaDPLMain::UnitTools);
