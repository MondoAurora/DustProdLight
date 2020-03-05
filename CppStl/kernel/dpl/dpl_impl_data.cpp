/*
 * dpl_impl_data.h
 *
 *	Value and Entity implementation (not too complicated)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl_data.h"

DustProdLightValue::~DustProdLightValue() {
}
;

void DustProdLightValue::set(DPLTokenType tokenType, const void* pVal) {
	this->tokenType = tokenType;

	switch (tokenType) {
	case DPL_TOKEN_VAL_BOOL:
		valInt = 0 != *((int*) pVal);
		break;
	case DPL_TOKEN_VAL_INT:
		valInt = *((int*) pVal);
		break;
	case DPL_TOKEN_VAL_DOUBLE:
		valDbl = *((double*) pVal);
		break;
	case DPL_TOKEN_VAL_STRING:
		valStr = *((string*) pVal);
		break;
	default:
		throw DPLErrInvalidValueType();
	}
}

void DustProdLightValue::optVisit(DPLVisitor *pVisitor, DPLEntity entity, DPLEntity token, void *pHint) {
	if (DPL_FILTER_SKIP != pVisitor->shouldProcess(entity, token)) {
		switch (tokenType) {
		case DPL_TOKEN_VAL_BOOL:
			pVisitor->processValBool(entity, token, valInt, pHint);
			break;
		case DPL_TOKEN_VAL_INT:
			pVisitor->processValInt(entity, token, valInt, pHint);
			break;
		case DPL_TOKEN_VAL_DOUBLE:
			pVisitor->processValDouble(entity, token, valDbl, pHint);
			break;
		case DPL_TOKEN_VAL_STRING:
			pVisitor->processValString(entity, token, valStr, pHint);
			break;
		default:
			throw DPLErrInvalidValueType();
		}
	}
}

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
