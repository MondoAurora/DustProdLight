#include "_dplgen_boot.h"
#include "dpl_stl.h"

#include <iostream>

#include <_dplgen_module_dpl_stl.h>

using namespace std;

void DPLMain::createBootEntities() {
//	DustProdLightRuntime::initMetaEntity(DPL_ENTITY_INVALID, DPL_ENTITY_INVALID, "!!INVALID!!");

	DustProdLightRuntime::initMetaEntity(DPL_MBI_STORE_SOURCE, DPL_TOKEN_STORE, "Source");

	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_TOOLS, DPL_TOKEN_UNIT, "Tools");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_MODEL, DPL_TOKEN_UNIT, "Model");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_IDEA, DPL_TOKEN_UNIT, "Idea");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_NARRATIVE, DPL_TOKEN_UNIT, "Narrative");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_DIALOG, DPL_TOKEN_UNIT, "Dialog");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_NATIVE, DPL_TOKEN_UNIT, "Native");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_DUST, DPL_TOKEN_UNIT, "Dust");

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_STORE, DPL_TOKEN_TYPE, "Store", DPL_MBI_UNIT_MODEL);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_UNIT, DPL_TOKEN_TYPE, "Unit", DPL_MBI_UNIT_MODEL);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_ENTITY, DPL_TOKEN_TYPE, "Entity", DPL_MBI_UNIT_MODEL);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_TYPE, DPL_TOKEN_TYPE, "Type", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_ATTRIBUTE, DPL_TOKEN_TYPE, "Attribute", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_REFERENCE, DPL_TOKEN_TYPE, "Reference", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_AGENT, DPL_TOKEN_TYPE, "Agent", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_TAG, DPL_TOKEN_TYPE, "Tag", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_CONSTANT, DPL_TOKEN_TYPE, "Constant", DPL_MBI_UNIT_IDEA);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_DUST_MODULE, DPL_TOKEN_TYPE, "Module", DPL_MBI_UNIT_NATIVE);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_NATIVE_SERVICE, DPL_TOKEN_TYPE, "Service", DPL_MBI_UNIT_NATIVE);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_NATIVE_COMMAND, DPL_TOKEN_TYPE, "Command", DPL_MBI_UNIT_NATIVE);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_TOOLS_CONNECTED, DPL_TOKEN_TYPE, "Connected", DPL_MBI_UNIT_TOOLS);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_ATT_ENTITY_GLOBALID, DPL_TOKEN_VAL_STRING, "GlobalId", DPL_MBI_TYPE_MODEL_ENTITY);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_CONNECTED_OWNER, DPL_TOKEN_REF_SINGLE, "Owner", DPL_MBI_TYPE_TOOLS_CONNECTED);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_CONNECTED_EXTENDS, DPL_TOKEN_REF_SET, "Extends", DPL_MBI_TYPE_TOOLS_CONNECTED);
}
