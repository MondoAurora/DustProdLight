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

const DPLEntity DPLUnitModel::UnitModel = DPL_MBI_UNIT_MODEL;
const DPLEntity DPLUnitModel::TypeStore = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Store", DPLUnitModel::UnitModel);
const DPLEntity DPLUnitModel::TypeUnit = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Unit", DPLUnitModel::UnitModel);

const DPLEntity DPLUnitModel::TypeEntity = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Entity", DPLUnitModel::UnitModel);
const DPLEntity DPLUnitModel::AttEntityGlobalId = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "GlobalId", DPLUnitModel::TypeEntity);


const DPLEntity DPLUnitIdea::UnitIdea = DPL_MBI_UNIT_IDEA;
const DPLEntity DPLUnitIdea::TypeType = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Type", DPLUnitIdea::UnitIdea);
const DPLEntity DPLUnitIdea::TypeAttribute = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Attribute", DPLUnitIdea::UnitIdea);
const DPLEntity DPLUnitIdea::TypeReference = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Reference", DPLUnitIdea::UnitIdea);

const DPLEntity DPLUnitNarrative::UnitNarrative = DPL_MBI_UNIT_NARRATIVE;

const DPLEntity DPLUnitNarrative::ActionSequence = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Sequence", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::ActionSelect = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Select", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::ActionRepeat = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Repeat", DPLUnitNarrative::UnitNarrative);

const DPLEntity DPLUnitNarrative::SvcProcessor = DPL_MBI_SVC_PROCESSOR;
const DPLEntity DPLUnitNarrative::CmdProcess = DPL_MBI_CMD_PROCESS;

const DPLEntity DPLUnitDialog::UnitDialog = DPL_MBI_UNIT_DIALOG;

const DPLEntity DPLUnitDialog::ActionDialogTokenRing = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "DialogTokenRing", DPLUnitDialog::UnitDialog);
const DPLEntity DPLUnitDialog::ActionAgent = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Agent", DPLUnitDialog::UnitDialog);
const DPLEntity DPLUnitDialog::ActionSignal = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Signal", DPLUnitDialog::UnitDialog);

const DPLEntity DPLUnitDialog::TypeDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPLUnitDialog::UnitDialog);
const DPLEntity DPLUnitDialog::AttDialogActiveAgent = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "ActiveAgentPos", DPLUnitDialog::TypeDialog);

const DPLEntity DPLUnitDialog::TypeAgent = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLUnitDialog::UnitDialog);
const DPLEntity DPLUnitDialog::RefAgentRelay = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Relay", DPLUnitDialog::TypeAgent);

const DPLEntity DPLUnitTools::UnitTools = DPL_MBI_UNIT_TOOLS;

const DPLEntity DPLUnitTools::TypeConnected = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Connected", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::RefConnectedOwner = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Owner", DPLUnitTools::TypeConnected);
const DPLEntity DPLUnitTools::RefConnectedExtends = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Extends", DPLUnitTools::TypeConnected);

const DPLEntity DPLUnitTools::TypeCollection = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Collection", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::RefCollectionMembers = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Members", DPLUnitTools::TypeCollection);
const DPLEntity DPLUnitTools::RefCollectionSeparator = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Separator", DPLUnitTools::TypeCollection);

const DPLEntity DPLUnitTools::TypeLink = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Link", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::RefLinkSource = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Source", DPLUnitTools::TypeLink);
const DPLEntity DPLUnitTools::RefLinkTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLUnitTools::TypeLink);


const DPLEntity DPLUnitText::UnitText = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Text");
const DPLEntity DPLUnitText::TypeCharacter = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Character", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttCharacterChar = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Char", DPLUnitText::TypeCharacter);
const DPLEntity DPLUnitText::TypeTextPlain = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "TextPlain", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttTextString = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "String", DPLUnitText::TypeTextPlain);

const DPLEntity DPLUnitTools::TypeStream = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Stream", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AttStreamPos = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Pos", DPLUnitTools::TypeStream);
const DPLEntity DPLUnitTools::AttStreamOK = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "StatusOK", DPLUnitTools::TypeStream);
const DPLEntity DPLUnitTools::AttStreamURL = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "URL", DPLUnitTools::TypeStream);

const DPLEntity DPLUnitTools::ActionDump = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "DumpChar", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::ActionReadStream = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "ReadStream", DPLUnitTools::UnitTools);
