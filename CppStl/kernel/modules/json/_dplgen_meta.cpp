
#include <_dplgen_module_json.h>

using namespace DPLUnitJson;

const DPLEntity DPLUnitJson::UnitJson = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Json");

const DPLEntity DPLUnitJson::AgentJsonParser = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "JsonParser", UnitJson);
