
#include <_dplgen_module_json.h>

using namespace DPLUnitJson;

const DPLEntity DPLUnitJson::UnitJson = DPLData::getMetaEntity(DPLUnitModel::TypeUnit, "Json");

const DPLEntity DPLUnitJson::AgentJsonParser = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "JsonParser", UnitJson);
