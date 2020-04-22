#include "dpl_stl.h"

using namespace std;

const DPLEntity DPLUnitModel::UnitModel = DPL_MBI_UNIT_MODEL;
const DPLEntity DPLUnitModel::TypeStore = DPL_MBI_TYPE_MODEL_STORE;
const DPLEntity DPLUnitModel::TypeUnit = DPL_MBI_TYPE_MODEL_UNIT;

const DPLEntity DPLUnitModel::TypeEntity = DPL_MBI_TYPE_MODEL_ENTITY;
const DPLEntity DPLUnitModel::AttEntityGlobalId = DPL_MBI_ATT_ENTITY_GLOBALID;
//const DPLEntity DPLUnitModel::RefEntityActions = DPLData::getMetaEntity(DPL_TOKEN_REF_MAP, "Actions", DPLUnitModel::TypeEntity);

const DPLEntity DPLUnitModel::ValEntityNull = DPLData::getConst(DPL_MBI_TYPE_IDEA_CONSTANT, "Null", UnitModel);
const DPLEntity DPLUnitModel::ValEntityFalse = DPLData::getConst(DPL_MBI_TYPE_IDEA_CONSTANT, "False", UnitModel);
const DPLEntity DPLUnitModel::ValEntityTrue = DPLData::getConst(DPL_MBI_TYPE_IDEA_CONSTANT, "True", UnitModel);


const DPLEntity DPLUnitIdea::UnitIdea = DPL_MBI_UNIT_IDEA;
const DPLEntity DPLUnitIdea::TypeType = DPL_MBI_TYPE_IDEA_TYPE;
//const DPLEntity DPLUnitIdea::RefTypeDefaultActions = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "DefaultActions", DPLUnitIdea::TypeType);
const DPLEntity DPLUnitIdea::TypeAttribute = DPL_MBI_TYPE_IDEA_ATTRIBUTE;
const DPLEntity DPLUnitIdea::TypeReference = DPL_MBI_TYPE_IDEA_REFERENCE;
const DPLEntity DPLUnitIdea::TypeAgent = DPL_MBI_TYPE_IDEA_AGENT;
const DPLEntity DPLUnitIdea::TypeTag = DPL_MBI_TYPE_IDEA_TAG;
const DPLEntity DPLUnitIdea::TypeConstant = DPL_MBI_TYPE_IDEA_CONSTANT;

const DPLEntity DPLUnitNarrative::UnitNarrative = DPL_MBI_UNIT_NARRATIVE;

const DPLEntity DPLUnitNarrative::TypeDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPL_MBI_UNIT_NARRATIVE);

//const DPLEntity DPLUnitNarrative::TypeAction = DPL_MBI_TYPE_NARRATIVE_ACTION;

//const DPLEntity DPLUnitNarrative::TypeAlgorithm = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Algorithm", DPLUnitNarrative::UnitNarrative);

const DPLEntity DPLUnitNarrative::AgentSequence = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Sequence", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::AgentSelect = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Select", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::AgentRepeat = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Repeat", DPLUnitNarrative::UnitNarrative);

//const DPLEntity DPLUnitNarrative::TypeExecAgent = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPLUnitNarrative::UnitNarrative);
//const DPLEntity DPLUnitNarrative::TypeExecDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPLUnitNarrative::UnitNarrative);

const DPLEntity DPLUnitNative::UnitNative = DPL_MBI_UNIT_NATIVE;

const DPLEntity DPLUnitNative::TypeModule = DPL_MBI_TYPE_DUST_MODULE;
const DPLEntity DPLUnitNative::RefModuleActions = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Actions", DPLUnitNative::TypeModule);
const DPLEntity DPLUnitNative::RefModuleAlgorithms = DPLData::getMetaEntity(DPL_TOKEN_REF_MAP, "Algorithms", DPLUnitNative::TypeModule);

const DPLEntity DPLUnitNative::TypeService = DPL_MBI_TYPE_NATIVE_SERVICE;
const DPLEntity DPLUnitNative::TypeCommand = DPL_MBI_TYPE_NATIVE_COMMAND;
const DPLEntity DPLUnitNative::TypeExecAtom = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Atom", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::RefExecAtomCommand = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Command", DPLUnitNative::TypeExecAtom);
const DPLEntity DPLUnitNative::RefExecAtomTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLUnitNative::TypeExecAtom);
const DPLEntity DPLUnitNative::RefExecAtomParam = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Param", DPLUnitNative::TypeExecAtom);

const DPLEntity DPLUnitNative::SvcAction = DPLData::getMetaEntity(DPL_TOKEN_SERVICE, "Action", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::CmdActionExecute = DPLData::getMetaEntity(DPL_TOKEN_COMMAND, "Execute", DPLUnitNative::SvcAction);

const DPLEntity DPLUnitNative::SvcResource = DPLData::getMetaEntity(DPL_TOKEN_SERVICE, "Resource", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::CmdResourceInit = DPLData::getMetaEntity(DPL_TOKEN_COMMAND, "Init", DPLUnitNative::SvcResource);
const DPLEntity DPLUnitNative::CmdResourceRelease = DPLData::getMetaEntity(DPL_TOKEN_COMMAND, "Release", DPLUnitNative::SvcResource);

const DPLEntity DPLUnitDialog::UnitDialog = DPL_MBI_UNIT_DIALOG;

//const DPLEntity DPLUnitDialog::TypeDialog = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Dialog", DPL_MBI_UNIT_DIALOG);


const DPLEntity DPLUnitDust::UnitDust = DPL_MBI_UNIT_DUST;

const DPLEntity DPLUnitDust::TypeRuntime = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Runtime", DPL_MBI_UNIT_DUST);
const DPLEntity DPLUnitDust::TypeSystem = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "System", DPL_MBI_UNIT_DUST);

const DPLEntity DPLUnitDust::TypeCore = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Core", DPL_MBI_UNIT_DUST);
//const DPLEntity DPLUnitDust::TypeAgent = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Agent", DPL_MBI_UNIT_DUST);
const DPLEntity DPLUnitDust::TypeBlock = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Block", DPL_MBI_UNIT_DUST);

const DPLEntity DPLUnitDust::RefRuntimeMain = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Main", DPLUnitDust::TypeRuntime);

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

const DPLEntity DPLUnitTools::TypeStream = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Stream", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AttStreamPos = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Pos", DPLUnitTools::TypeStream);
const DPLEntity DPLUnitTools::AttStreamOK = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "StatusOK", DPLUnitTools::TypeStream);
const DPLEntity DPLUnitTools::AttStreamURL = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "URL", DPLUnitTools::TypeStream);

const DPLEntity DPLUnitTools::AgentDump = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "DumpChar", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AgentReadStream = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "ReadStream", DPLUnitTools::UnitTools);

const DPLEntity DPLUnitTools::TypeLimits = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Limits", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AttLimitsIntMin = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "IntMin", DPLUnitTools::TypeLimits);
const DPLEntity DPLUnitTools::AttLimitsIntMax = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "IntMax", DPLUnitTools::TypeLimits);

