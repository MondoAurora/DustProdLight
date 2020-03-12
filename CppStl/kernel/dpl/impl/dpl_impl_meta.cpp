/*
 * dpl_impl_meta.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl_meta.h"

using namespace std;

const DPLEntity DPLImplMeta::UnitModel = DPL_MBI_UNIT_MODEL;
const DPLEntity DPLImplMeta::TypeStore = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Store", DPLImplMeta::UnitModel);
const DPLEntity DPLImplMeta::TypeUnit = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Unit", DPLImplMeta::UnitModel);

const DPLEntity DPLImplMeta::TypeEntity = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Entity", DPLImplMeta::UnitModel);
const DPLEntity DPLImplMeta::AttEntityGlobalId = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "GlobalId", DPLImplMeta::TypeEntity);


const DPLEntity DPLImplMeta::UnitIdea = DPL_MBI_UNIT_IDEA;
const DPLEntity DPLImplMeta::TypeType = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Type", DPLImplMeta::UnitIdea);
const DPLEntity DPLImplMeta::TypeAttribute = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Attribute", DPLImplMeta::UnitIdea);
const DPLEntity DPLImplMeta::TypeReference = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Reference", DPLImplMeta::UnitIdea);

const DPLEntity DPLImplMeta::UnitNarrative = DPL_MBI_UNIT_NARRATIVE;

const DPLEntity DPLImplMeta::ActionSequence = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Sequence", DPLImplMeta::UnitNarrative);
const DPLEntity DPLImplMeta::ActionSelect = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Select", DPLImplMeta::UnitNarrative);
const DPLEntity DPLImplMeta::ActionRepeat = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "Repeat", DPLImplMeta::UnitNarrative);

const DPLEntity DPLImplMeta::UnitDialog = DPL_MBI_UNIT_DIALOG;

const DPLEntity DPLImplMeta::UnitTools = DPL_MBI_UNIT_TOOLS;

const DPLEntity DPLImplMeta::TypeConnected = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Connected", DPLImplMeta::UnitTools);
const DPLEntity DPLImplMeta::RefConnectedOwner = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Owner", DPLImplMeta::TypeConnected);
const DPLEntity DPLImplMeta::RefConnectedExtends = DPLData::getMetaEntity(DPL_TOKEN_REF_SET, "Extends", DPLImplMeta::TypeConnected);

