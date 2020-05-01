#include "dpl_stl.h"

using namespace std;

const DPLEntity DPLUnitModel::UnitModel = DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_MODEL, DPL_TOKEN_UNIT, "Model");

const DPLEntity DPLUnitModel::TypeStore = DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_STORE, DPL_TOKEN_TYPE, "Store", DPL_MBI_UNIT_MODEL);
const DPLEntity DPLUnitModel::TypeUnit = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_UNIT, DPL_TOKEN_TYPE, "Unit", DPL_MBI_UNIT_MODEL);
const DPLEntity DPLUnitModel::TypeEntity = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Entity", DPL_MBI_UNIT_MODEL);

const DPLEntity DPLUnitModel::AttEntityGlobalId = DustProdLightRuntime::initMetaEntity(DPL_MBI_ATT_ENTITY_GLOBALID, DPL_TOKEN_VAL_STRING, "GlobalId", DPL_MBI_UNIT_MODEL);

const DPLEntity DPLUnitModel::ValEntityNull = DPLData::getMetaEntity(DPL_TOKEN_CONST, "Null", UnitModel);
const DPLEntity DPLUnitModel::ValEntityFalse = DustProdLightRuntime::initMetaEntity(DPL_ENTITY_FALSE, DPL_TOKEN_CONST, "False", UnitModel);
const DPLEntity DPLUnitModel::ValEntityTrue = DustProdLightRuntime::initMetaEntity(DPL_ENTITY_TRUE, DPL_TOKEN_CONST, "True", UnitModel);


const DPLEntity DPLUnitIdea::UnitIdea = DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_IDEA, DPL_TOKEN_UNIT, "Idea");

const DPLEntity DPLUnitIdea::TypeType = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_TYPE, DPL_TOKEN_TYPE, "Type");
//const DPLEntity DPLUnitIdea::RefTypeAttributes = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Attributes", DPL_TOKEN_TYPE);
//const DPLEntity DPLUnitIdea::RefTypeReferences = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "References", DPL_TOKEN_TYPE);

const DPLEntity DPLUnitIdea::TypeAttBool = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_VAL_BOOL, DPL_TOKEN_TYPE, "Bool");
const DPLEntity DPLUnitIdea::TypeAttInt = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_VAL_INT, DPL_TOKEN_TYPE, "Int");
const DPLEntity DPLUnitIdea::TypeAttDouble = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_VAL_DOUBLE, DPL_TOKEN_TYPE, "Double");
const DPLEntity DPLUnitIdea::TypeAttString = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_VAL_STRING, DPL_TOKEN_TYPE, "String");

const DPLEntity DPLUnitIdea::TypeRefSingle = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_REF_SINGLE, DPL_TOKEN_TYPE, "Single");
const DPLEntity DPLUnitIdea::TypeRefSet = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_REF_SET, DPL_TOKEN_TYPE, "Set");
const DPLEntity DPLUnitIdea::TypeRefArr = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_REF_ARR, DPL_TOKEN_TYPE, "Arr");
const DPLEntity DPLUnitIdea::TypeRefMap = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_REF_MAP, DPL_TOKEN_TYPE, "Map");

const DPLEntity DPLUnitIdea::TypeConst = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_CONST, DPL_TOKEN_TYPE, "Const");
const DPLEntity DPLUnitIdea::TypeTag = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_TAG, DPL_TOKEN_TYPE, "Tag");
const DPLEntity DPLUnitIdea::TypeAgent = DustProdLightRuntime::initMetaEntity(DPL_TOKEN_AGENT, DPL_TOKEN_TYPE, "Agent");


const DPLEntity DPLUnitNarrative::UnitNarrative = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Narrative");

const DPLEntity DPLUnitNarrative::AgentDialog = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Dialog", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::AgentSequence = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Sequence", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::AgentSelect = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Select", DPLUnitNarrative::UnitNarrative);
const DPLEntity DPLUnitNarrative::AgentRepeat = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Repeat", DPLUnitNarrative::UnitNarrative);


const DPLEntity DPLUnitDialog::UnitDialog = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Dialog");


const DPLEntity DPLUnitNative::UnitNative = DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_NATIVE, DPL_TOKEN_UNIT, "Native");

const DPLEntity DPLUnitNative::TypeModule = DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_NATIVE_MODULE, DPL_TOKEN_TYPE, "Module", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::RefModuleAgents = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Agents", DPLUnitNative::TypeModule);
const DPLEntity DPLUnitNative::RefModuleAlgorithms = DPLData::getMetaEntity(DPL_TOKEN_REF_MAP, "Algorithms", DPLUnitNative::TypeModule);

const DPLEntity DPLUnitNative::TypeService = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Service", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::TypeCommand = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Command", DPLUnitNative::UnitNative);

const DPLEntity DPLUnitNative::SvcAction = DPLData::getMetaEntity(DPLUnitNative::TypeService, "Action", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::CmdActionExecute = DPLData::getMetaEntity(DPLUnitNative::TypeCommand, "Execute", DPLUnitNative::SvcAction);

const DPLEntity DPLUnitNative::SvcResource = DPLData::getMetaEntity(DPLUnitNative::TypeService, "Resource", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::CmdResourceInit = DPLData::getMetaEntity(DPLUnitNative::TypeCommand, "Init", DPLUnitNative::SvcResource);
const DPLEntity DPLUnitNative::CmdResourceRelease = DPLData::getMetaEntity(DPLUnitNative::TypeCommand, "Release", DPLUnitNative::SvcResource);

const DPLEntity DPLUnitNative::SvcLogic = DPLData::getMetaEntity(DPLUnitNative::TypeService, "Logic", DPLUnitNative::UnitNative);

const DPLEntity DPLUnitNative::AgentCall = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Call", DPLUnitNative::UnitNative);
const DPLEntity DPLUnitNative::RefAgentCallCommand = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Command", DPLUnitNative::AgentCall);
const DPLEntity DPLUnitNative::RefAgentCallTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLUnitNative::AgentCall);
const DPLEntity DPLUnitNative::RefAgentCallParam = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Param", DPLUnitNative::AgentCall);

const DPLEntity DPLUnitDust::UnitDust = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Dust");

const DPLEntity DPLUnitDust::AgentRuntime = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Runtime", DPLUnitDust::UnitDust);
const DPLEntity DPLUnitDust::AgentSystem = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "System", DPLUnitDust::UnitDust);

const DPLEntity DPLUnitDust::AgentCore = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Core", DPLUnitDust::UnitDust);
const DPLEntity DPLUnitDust::AgentBlock = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "Block", DPLUnitDust::UnitDust);

const DPLEntity DPLUnitDust::RefRuntimeMain = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Main", DPLUnitDust::AgentRuntime);

const DPLEntity DPLUnitTools::UnitTools = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Tools");

const DPLEntity DPLUnitTools::TypeConnected = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Connected", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::RefConnectedOwner = DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_CONNECTED_OWNER, DPL_TOKEN_REF_SINGLE, "Owner", DPLUnitTools::TypeConnected);
const DPLEntity DPLUnitTools::RefConnectedExtends = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Extends", DPLUnitTools::TypeConnected);

const DPLEntity DPLUnitTools::TypeCollection = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Collection", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::RefCollectionMembers = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Members", DPLUnitTools::TypeCollection);
const DPLEntity DPLUnitTools::RefCollectionSeparator = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Separator", DPLUnitTools::TypeCollection);

const DPLEntity DPLUnitTools::TypeLink = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Link", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::RefLinkSource = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Source", DPLUnitTools::TypeLink);
const DPLEntity DPLUnitTools::RefLinkTarget = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Target", DPLUnitTools::TypeLink);

const DPLEntity DPLUnitTools::TypeStream = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Stream", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AttStreamPos = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Pos", DPLUnitTools::TypeStream);
const DPLEntity DPLUnitTools::AttStreamOK = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "StatusOK", DPLUnitTools::TypeStream);
const DPLEntity DPLUnitTools::AttStreamURL = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "URL", DPLUnitTools::TypeStream);

const DPLEntity DPLUnitTools::AgentDump = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "DumpChar", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AgentReadStream = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "ReadStream", DPLUnitTools::UnitTools);

const DPLEntity DPLUnitTools::TypeLimits = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Limits", DPLUnitTools::UnitTools);
const DPLEntity DPLUnitTools::AttLimitsIntMin = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "IntMin", DPLUnitTools::TypeLimits);
const DPLEntity DPLUnitTools::AttLimitsIntMax = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "IntMax", DPLUnitTools::TypeLimits);

