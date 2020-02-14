/*
 * fleetmandata.cpp
 *
 *  Created on: Feb 12, 2020
 *      Author: loran
 */

#include "dpl_impl.h"

DPLStore* DPLStore::pTheStore;
DustProdLightStore DustProdLightStore::store;

DustProdLightStore::DustProdLightStore() {
	DPLStore::pTheStore = this;

	// create zero index placeholders in all collectors; means invalid / nonexistent
	DPLType notype = getType("");
	getTokenInt(notype, "", fmd_valtype_invalid);
	createEntity(notype);
}

DustProdLightStore::~DustProdLightStore() {
}

DustProdLightToken *DustProdLightStore::validateGetToken(DPLToken token, DPLValType valType) {
	DustProdLightToken *pToken = vecTokens[token];
	if (pToken->valType != valType) {
		throw DPLErrInvalidValueType();
	}
	return pToken;
}

DustProdLightToken *DustProdLightStore::getTokenImpl(DPLType type, string tokenName, DPLValType valType) {
	DustProdLightToken *pToken;

	string id = vecTypes[type] + "." + tokenName;

	pToken = &mapTokens[id];

	if (-1 == pToken->id) {
		pToken->id = vecTokens.size();
		pToken->valType = valType;

		vecTokens.push_back(pToken);
	} else {
		if (pToken->valType != valType) {
			throw DPLErrInvalidValueType();
		}
	}

	return pToken;
}

DPLType DustProdLightStore::getTypeInt(string typeName) {
	int ret;

	if (mapTypes.end() == mapTypes.find(typeName)) {
		ret = vecTypes.size();
		mapTypes[typeName] = ret;
		vecTypes.push_back(typeName);
	} else {
		ret = mapTypes[typeName];
	}

	return ret;
}

DPLToken DustProdLightStore::getTokenInt(DPLType type, string tokenName, DPLValType valType) {
	return getTokenImpl(type, tokenName, valType)->id;
}

DPLValue *DustProdLightStore::getValue(DPLEntity entity, DPLValType valType, DPLToken token) {
	store.validateGetToken(token, valType);
	DustProdLightEntity *pEntity = &store.dataLocal[entity];
	return (pEntity->values.end() == pEntity->values.find(token)) ? 0 : &pEntity->values[token];
}

void DustProdLightStore::setValue(DPLEntity entity, DPLToken token, DPLValType valType, void* pVal) {
	validateGetToken(token, valType);
	DustProdLightEntity *pEntity = &store.dataLocal[entity];
	pEntity->values[token].set(valType, pVal);
}

bool DustProdLightStore::chgRef(DPLChange chg, DPLEntity entity, DPLToken token, DPLEntity target, int key) {
	bool ret = false;

	DustProdLightEntity *pEntity = &store.dataLocal[entity];
	DustProdLightRef *pRef = pEntity->refs[token];

	if (pRef) {
		if (!pRef->getBy(target, key)) {
			new DustProdLightRef(pRef, target, key);
		}
	} else {
		pEntity->refs[token] = new DustProdLightRef(vecTokens[token], entity, target, key);
	}

	return ret;
}

DPLRef* DustProdLightStore::getRef(DPLEntity entity, DPLToken token) {
	DustProdLightEntity *pEntity = &store.dataLocal[entity];
	DustProdLightRef *pRef = pEntity->refs[token];

	return pRef;
}

DPLEntity DustProdLightStore::createEntityInt(DPLType primaryType) {
	DPLEntity ret = nextEntityId;
	DustProdLightEntity *pEntity = &dataLocal[nextEntityId++];
	pEntity->localId = ret;
	pEntity->primaryType = primaryType;
	return ret;
}

DPLEntityInt* DustProdLightStore::getEntityInt(DPLEntity entity) {
	return &store.dataLocal[entity];
}
