/*
 * dpl_impl_meta.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include <_dplgen_module_dpl_stl.h>

#include "_dplgen_boot.h"

using namespace std;

const DPLEntity DPLUnitModel::UnitModel = DPL_MBI_UNIT_MODEL;
const DPLEntity DPLUnitModel::TypeStore = DPL_MBI_TYPE_MODEL_STORE;
const DPLEntity DPLUnitModel::TypeUnit = DPL_MBI_TYPE_MODEL_UNIT;

const DPLEntity DPLUnitModel::TypeEntity = DPL_MBI_TYPE_MODEL_ENTITY;
const DPLEntity DPLUnitModel::AttEntityGlobalId = DPL_MBI_ATT_ENTITY_GLOBALID;
const DPLEntity DPLUnitModel::RefEntityActions = DPLData::getMetaEntity(DPL_TOKEN_REF_MAP, "Actions", DPLUnitModel::TypeEntity);


const DPLEntity DPLUnitIdea::UnitIdea = DPL_MBI_UNIT_IDEA;
const DPLEntity DPLUnitIdea::TypeType = DPL_MBI_TYPE_IDEA_TYPE;
const DPLEntity DPLUnitIdea::RefTypeDefaultActions = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "DefaultActions", DPLUnitIdea::TypeType);
const DPLEntity DPLUnitIdea::TypeAttribute = DPL_MBI_TYPE_IDEA_ATTRIBUTE;
const DPLEntity DPLUnitIdea::TypeReference = DPL_MBI_TYPE_IDEA_REFERENCE;
const DPLEntity DPLUnitIdea::TypeTag = DPL_MBI_TYPE_IDEA_TAG;

const DPLEntity DPLUnitNarrative::UnitNarrative = DPL_MBI_UNIT_NARRATIVE;

const DPLEntity DPLUnitNarrative::TypeAction = DPL_MBI_TYPE_NARRATIVE_ACTION;

const DPLEntity DPLUnitNarrative::TypeCtrlSequence = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Sequence", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::TypeCtrlSelect = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Select", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::TypeCtrlRepeat = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Repeat", DPLUnitNarrative::UnitNarrative);

const DPLEntity DPLUnitNarrative::TypeExecAtom = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Atom", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::RefExecAtomCommand = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Command", DPLUnitNarrative::TypeExecAtom);
const DPLEntity DPLUnitNarrative::RefExecAtomTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLUnitNarrative::TypeExecAtom);
const DPLEntity DPLUnitNarrative::RefExecAtomParam = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Param", DPLUnitNarrative::TypeExecAtom);

const DPLEntity DPLUnitNarrative::TypeExecAgent = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::TypeExecDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPLUnitNarrative::UnitNarrative);


const DPLEntity DPLUnitNarrative::SvcAction = DPLData::getMetaEntity(DPL_TOKEN_SERVICE, "Action", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::CmdActionExecute = DPLData::getMetaEntity(DPL_TOKEN_COMMAND, "Execute", DPLUnitNarrative::SvcAction);

const DPLEntity DPLUnitNarrative::SvcResource = DPLData::getMetaEntity(DPL_TOKEN_SERVICE, "Resource", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::CmdResourceInit = DPLData::getMetaEntity(DPL_TOKEN_COMMAND, "Init", DPLUnitNarrative::SvcResource);
const DPLEntity DPLUnitNarrative::CmdResourceRelease = DPLData::getMetaEntity(DPL_TOKEN_COMMAND, "Release", DPLUnitNarrative::SvcResource);

const DPLEntity DPLUnitDialog::UnitDialog = DPL_MBI_UNIT_DIALOG;

const DPLEntity DPLUnitDust::UnitDust = DPL_MBI_UNIT_DUST;

const DPLEntity DPLUnitDust::TypeSystem = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "System", DPL_MBI_UNIT_DUST);
const DPLEntity DPLUnitDust::TypeModule = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Module", DPL_MBI_UNIT_DUST);
const DPLEntity DPLUnitDust::TypeRuntime = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Runtime", DPL_MBI_UNIT_DUST);
const DPLEntity DPLUnitDust::TypeBlock = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Block", DPL_MBI_UNIT_DUST);

const DPLEntity DPLUnitDust::RefRuntimeMain = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Main", DPLUnitDust::TypeRuntime);

const DPLEntity DPLUnitDust::ActionCtrlSequence = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Sequence", DPLUnitDust::UnitDust);
const DPLEntity DPLUnitDust::ActionCtrlSelect = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Select", DPLUnitDust::UnitDust);
const DPLEntity DPLUnitDust::ActionCtrlRepeat = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Repeat", DPLUnitDust::UnitDust);

//const DPLEntity DPLUnitDust::ActionExecAtom = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Atom", DPLUnitDust::UnitDust);
//const DPLEntity DPLUnitDust::ActionExecAgent = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Agent", DPLUnitDust::UnitDust);
//const DPLEntity DPLUnitDust::ActionExecDialog = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Dialog", DPLUnitDust::UnitDust);

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

const DPLEntity DPLUnitTools::TypeLimits = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Limits", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AttLimitsIntMin = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "IntMin", DPLUnitTools::TypeLimits);
const DPLEntity DPLUnitTools::AttLimitsIntMax = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "IntMax", DPLUnitTools::TypeLimits);

