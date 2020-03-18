/*
 * dpl_impl_data.h
 *
 *	Value and Entity implementation (not too complicated)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include "data.h"
#include "../dpl_impl.h"

void DustProdLightEntity::updated() {
	changed = true;
}

void DustProdLightEntity::optReloadMeta() {
	if (changed) {
		types.clear();

		for (EntityValIterator iter = values.begin(); iter != values.end(); ++iter) {
			int k = iter->first;
//			types.insert(DustProdLightStore::store->vecTokens[k]->type);
		}
		for (EntityRefIterator iter = refs.begin(); iter != refs.end(); ++iter) {
			int k = iter->first;
//			types.insert(DustProdLightStore::store->vecTokens[k]->type);
		}
		changed = false;
	}
}

DustProdLightEntity::~DustProdLightEntity() {
}

bool DustProdLightEntity::isOfType(DPLEntity type) {
	return types.end() != types.find(type);
}

void DustProdLightEntity::getAllTypes(set<DPLEntity>& typeSet) {
	typeSet.clear();
	typeSet.insert(types.begin(), types.end());
}

void* DustProdLightEntity::optVisit(DPLVisitor *pVisitor, int key, void *pHint) {
	void *pRet = pHint;
	DPLFilterResponse fr = pVisitor->shouldProcess(localId, key);

	if (DPL_FILTER_SKIP != fr) {
		pRet = pVisitor->processBeginEntity(localId, 0, pHint);

		for (EntityValIterator iter = values.begin(); iter != values.end(); ++iter) {
			int token = iter->first;
			iter->second.optVisit(pVisitor, localId, token, pRet);
		}

		for (EntityRefIterator iter = refs.begin(); iter != refs.end(); ++iter) {
			iter->second->optVisit(pVisitor, pRet);
		}
		pRet = pVisitor->processEndEntity(localId, 0, pHint);
	}

	return pRet;
}

string DustProdLightImplementation::getString(DustProdLightEntity *pEntity, DPLEntity token) {
	return pEntity->values[token].valStr;
}

void DustProdLightImplementation::initMetaEntity(DustProdLightEntity *pEntity, DPLEntity entity, DPLTokenType tokenType, const char* id,
		DPLEntity parent) {
	pEntity->localId = entity;
	pEntity->tokenType = tokenType;

	switch (tokenType) {
	case DPL_ENTITY_INVALID:
		pEntity->primaryType = DPL_ENTITY_INVALID;
		break;
	case DPL_TOKEN_STORE:
		pEntity->primaryType = DPL_MBI_TYPE_MODEL_STORE;
		break;
	case DPL_TOKEN_UNIT:
		pEntity->primaryType = DPL_MBI_TYPE_MODEL_UNIT;
		break;
	case DPL_TOKEN_TYPE:
		pEntity->primaryType = DPL_MBI_TYPE_IDEA_TYPE;
		break;
	case DPL_TOKEN_VAL_BOOL:
	case DPL_TOKEN_VAL_DOUBLE:
	case DPL_TOKEN_VAL_INT:
	case DPL_TOKEN_VAL_STRING:
		pEntity->primaryType = DPL_MBI_TYPE_IDEA_ATTRIBUTE;
		break;
	case DPL_TOKEN_REF_SINGLE:
	case DPL_TOKEN_REF_SET:
	case DPL_TOKEN_REF_ARR:
	case DPL_TOKEN_REF_MAP:
		pEntity->primaryType = DPL_MBI_TYPE_IDEA_REFERENCE;
		break;
	default:
		pEntity->primaryType = DPL_ENTITY_INVALID;
		break;
	}

	pEntity->types.insert(pEntity->primaryType);
	pEntity->values[DPL_MBI_ATT_ENTITY_GLOBALID].set(DPL_TOKEN_VAL_STRING, &id);
}
