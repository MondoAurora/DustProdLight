/*
 * dpl_impl_meta.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "_dplgen_boot.h"
#include "dpl_stl.h"

#include <iostream>

#include <_dplgen_module_dpl_stl.h>

using namespace std;

void DPLMain::createBootEntities() {
	DustProdLightRuntime::initMetaEntity(DPL_ENTITY_INVALID, DPL_ENTITY_INVALID, "!!INVALID!!");

	DustProdLightRuntime::initMetaEntity(DPL_MBI_STORE_SOURCE, DPL_TOKEN_STORE, "Source");

	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_TOOLS, DPL_TOKEN_UNIT, "Tools");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_MODEL, DPL_TOKEN_UNIT, "Model");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_IDEA, DPL_TOKEN_UNIT, "Idea");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_NARRATIVE, DPL_TOKEN_UNIT, "Narrative");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_DIALOG, DPL_TOKEN_UNIT, "Dialog");

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_STORE, DPL_TOKEN_TYPE, "Store", DPL_MBI_UNIT_MODEL);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_UNIT, DPL_TOKEN_TYPE, "Unit", DPL_MBI_UNIT_MODEL);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_MODEL_ENTITY, DPL_TOKEN_TYPE, "Entity", DPL_MBI_UNIT_MODEL);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_TYPE, DPL_TOKEN_TYPE, "Type", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_ATTRIBUTE, DPL_TOKEN_TYPE, "Attribute", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_REFERENCE, DPL_TOKEN_TYPE, "Reference", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_SERVICE, DPL_TOKEN_TYPE, "Service", DPL_MBI_UNIT_IDEA);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_IDEA_COMMAND, DPL_TOKEN_TYPE, "Command", DPL_MBI_UNIT_IDEA);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_NARRATIVE_ACTION, DPL_TOKEN_TYPE, "Action", DPL_MBI_UNIT_NARRATIVE);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_DIALOG_TASK, DPL_TOKEN_TYPE, "Task", DPL_MBI_UNIT_DIALOG);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_TASK_COMMAND, DPL_TOKEN_REF_SINGLE, "Command", DPL_MBI_TYPE_DIALOG_TASK);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_TASK_TARGET, DPL_TOKEN_REF_SINGLE, "Target", DPL_MBI_TYPE_DIALOG_TASK);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_TOOLS_CONNECTED, DPL_TOKEN_TYPE, "Connected", DPL_MBI_UNIT_TOOLS);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_ATT_ENTITY_GLOBALID, DPL_TOKEN_VAL_STRING, "GlobalId", DPL_MBI_TYPE_MODEL_ENTITY);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_CONNECTED_OWNER, DPL_TOKEN_REF_SINGLE, "Owner", DPL_MBI_TYPE_TOOLS_CONNECTED);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_CONNECTED_EXTENDS, DPL_TOKEN_REF_SET, "Extends", DPL_MBI_TYPE_TOOLS_CONNECTED);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_SVC_PROCESSOR, DPL_TOKEN_SERVICE, "Processor", DPL_MBI_UNIT_NARRATIVE);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_CMD_PROCESS, DPL_TOKEN_COMMAND, "Process", DPL_MBI_SVC_PROCESSOR);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_UNIT_DUST, DPL_TOKEN_UNIT, "Dust");
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_DUST_SYSTEM, DPL_TOKEN_TYPE, "System", DPL_MBI_UNIT_DUST);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_DUST_MODULE, DPL_TOKEN_TYPE, "Module", DPL_MBI_UNIT_DUST);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_DUST_RUNTIME, DPL_TOKEN_TYPE, "Runtime", DPL_MBI_UNIT_DUST);
	DustProdLightRuntime::initMetaEntity(DPL_MBI_TYPE_DUST_BLOCK, DPL_TOKEN_TYPE, "Block", DPL_MBI_UNIT_DUST);

	DustProdLightRuntime::initMetaEntity(DPL_MBI_REF_RUNTIME_MAIN, DPL_TOKEN_REF_SINGLE, "Main", DPL_MBI_TYPE_DUST_RUNTIME);
}
