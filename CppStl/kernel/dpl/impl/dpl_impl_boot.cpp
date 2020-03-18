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

void DustProdLightImplementation::createBootEntities() {
	initMetaEntity(DPL_ENTITY_INVALID, DPL_ENTITY_INVALID, "!!INVALID!!");

	initMetaEntity(DPL_MBI_STORE_SOURCE, DPL_TOKEN_STORE, "Source");

	initMetaEntity(DPL_MBI_UNIT_TOOLS, DPL_TOKEN_UNIT, "Tools");
	initMetaEntity(DPL_MBI_UNIT_MODEL, DPL_TOKEN_UNIT, "Model");
	initMetaEntity(DPL_MBI_UNIT_IDEA, DPL_TOKEN_UNIT, "Idea");
	initMetaEntity(DPL_MBI_UNIT_NARRATIVE, DPL_TOKEN_UNIT, "Narrative");
	initMetaEntity(DPL_MBI_UNIT_DIALOG, DPL_TOKEN_UNIT, "Dialog");

	initMetaEntity(DPL_MBI_TYPE_MODEL_STORE, DPL_TOKEN_TYPE, "Store", DPL_MBI_UNIT_MODEL);
	initMetaEntity(DPL_MBI_TYPE_MODEL_UNIT, DPL_TOKEN_TYPE, "Unit", DPL_MBI_UNIT_MODEL);
	initMetaEntity(DPL_MBI_TYPE_MODEL_ENTITY, DPL_TOKEN_TYPE, "Entity", DPL_MBI_UNIT_MODEL);

	initMetaEntity(DPL_MBI_TYPE_IDEA_TYPE, DPL_TOKEN_TYPE, "Type", DPL_MBI_UNIT_IDEA);
	initMetaEntity(DPL_MBI_TYPE_IDEA_ATTRIBUTE, DPL_TOKEN_TYPE, "Attribute", DPL_MBI_UNIT_IDEA);
	initMetaEntity(DPL_MBI_TYPE_IDEA_REFERENCE, DPL_TOKEN_TYPE, "Reference", DPL_MBI_UNIT_IDEA);

	initMetaEntity(DPL_MBI_TYPE_TOOLS_CONNECTED, DPL_TOKEN_TYPE, "Connected", DPL_MBI_UNIT_TOOLS);

	initMetaEntity(DPL_MBI_ATT_ENTITY_GLOBALID, DPL_TOKEN_VAL_STRING, "GlobalId", DPL_MBI_TYPE_MODEL_ENTITY);

	initMetaEntity(DPL_MBI_REF_CONNECTED_OWNER, DPL_TOKEN_REF_SINGLE, "Owner", DPL_MBI_TYPE_TOOLS_CONNECTED);
	initMetaEntity(DPL_MBI_REF_CONNECTED_EXTENDS, DPL_TOKEN_REF_SET, "Extends", DPL_MBI_TYPE_TOOLS_CONNECTED);
}
