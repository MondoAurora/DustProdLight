/*
 * dpl_impl_store.cpp
 *
 *	Implementation of the Store and the DPL static API
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */
#include <iostream>

#include "dpl_impl.h"

DustProdLightStore* DustProdLightStore::store = NULL;

DustProdLightStore::DustProdLightStore() {
// create zero index placeholders in all collectors; means invalid / nonexistent
	DPLType notype = getType("");
	getToken(notype, "", DPL_TOKEN_INVALID);
	createEntity(notype);
	cout << "DustProdLightStore - Initialized" << endl;
}

DustProdLightStore::~DustProdLightStore() {
	cout << "DustProdLightStore - Shutdown" << endl;
}

DPLType DustProdLightStore::getType(string typeName) {
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

DustProdLightToken *DustProdLightStore::validateGetToken(DPLToken token, DPLTokenType tokenType) {
	DustProdLightToken *pToken = vecTokens[token];
	if (pToken->tokenType != tokenType) {
		throw DPLErrInvalidValueType();
	}
	return pToken;
}

DustProdLightToken *DustProdLightStore::getToken(DPLType type, string tokenName, DPLTokenType tokenType) {
	DustProdLightToken *pToken;

	string id = vecTypes[type] + "." + tokenName;

	pToken = &mapTokens[id];

	if (-1 == pToken->id) {
		pToken->id = vecTokens.size();
		pToken->tokenType = tokenType;
		pToken->name = tokenName;
		pToken->idStr = id;

		vecTokens.push_back(pToken);
	} else {
		if (pToken->tokenType != tokenType) {
			throw DPLErrInvalidValueType();
		}
	}

	return pToken;
}

DustProdLightValue *DustProdLightStore::getValue(DPLEntity entity, DPLTokenType tokenType, DPLToken token) {
	store->validateGetToken(token, tokenType);
	DustProdLightEntity *pEntity = &store->dataLocal[entity];
	return (pEntity->values.end() == pEntity->values.find(token)) ? 0 : &pEntity->values[token];
}

void DustProdLightStore::setValue(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void* pVal) {
	validateGetToken(token, tokenType);
	DustProdLightEntity *pEntity = &store->dataLocal[entity];
	pEntity->values[token].set(tokenType, pVal);
}

bool DustProdLightStore::chgRef(DPLChange chg, DPLEntity entity, DPLToken token, DPLEntity target, int key) {
	bool ret = false;

	DustProdLightEntity *pEntity = &store->dataLocal[entity];
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

DustProdLightRef* DustProdLightStore::getRef(DPLEntity entity, DPLToken token) {
	DustProdLightEntity *pEntity = &store->dataLocal[entity];
	DustProdLightRef *pRef = pEntity->refs[token];

	return pRef;
}

DustProdLightEntity *DustProdLightStore::createEntity(DPLType primaryType) {
	DPLEntity ret = nextEntityId;
	DustProdLightEntity *pEntity = &dataLocal[nextEntityId++];
	pEntity->localId = ret;
	pEntity->primaryType = primaryType;
	return pEntity;
}

DustProdLightEntity* DustProdLightStore::getEntity(DPLEntity entity) {
	return &store->dataLocal[entity];
}

/******************************************************
 *
 * Implementing DPL functions with DustProdLightStore
 *
 ******************************************************/

DPLType DPLMeta::getType(string typeName) {
	init();

	return DustProdLightStore::store->getType(typeName);
}

DPLToken DPLMeta::getToken(DPLType type, string tokenName, DPLTokenType tokenType) {
	DustProdLightToken* pToken = DustProdLightStore::store->getToken(type, tokenName, tokenType);
	return pToken->id;
}

DPLToken DPLMeta::getToken(string tokenId) {
	map<string, DustProdLightToken>::iterator i = DustProdLightStore::store->mapTokens.find(tokenId);
	return (DustProdLightStore::store->mapTokens.end() == i) ? 0 : i->second.id;
}

void DPLData::setBool(DPLEntity entity, DPLToken token, bool b) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_BOOL, &b);
}

void DPLData::setInt(DPLEntity entity, DPLToken token, int i) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_INT, &i);
}

bool DPLData::getBool(DPLEntity entity, DPLToken token, bool defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_BOOL, token);
	return (0 == pVal) ? defValue : pVal->valInt;
}

int DPLData::getInt(DPLEntity entity, DPLToken token, int defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_INT, token);
	return (0 == pVal) ? defValue : pVal->valInt;
}

void DPLData::setDouble(DPLEntity entity, DPLToken token, double d) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_DOUBLE, &d);
}
double DPLData::getDouble(DPLEntity entity, DPLToken token, double defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_DOUBLE, token);
	return (0 == pVal) ? defValue : pVal->valDbl;
}

void DPLData::setString(DPLEntity entity, DPLToken token, string s) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_STRING, &s);

}
string DPLData::getString(DPLEntity entity, DPLToken token, string defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_STRING, token);
	return (0 == pVal) ? defValue : pVal->valStr;
}

// references
bool DPLData::setRef(DPLEntity entity, DPLToken token, DPLEntity target, int key) {
	return DustProdLightStore::store->chgRef(DPL_CHG_REF_SET, entity, token, target, key);
}

int DPLData::getRefCount(DPLEntity entity, DPLToken token) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return pR ? pR->getCount() : 0;
}

DPLEntity DPLData::getRef(DPLEntity entity, DPLToken token, int key) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return pR ? pR->getRef(key) : 0;
}

DPLToken DPLData::getRefKey(DPLEntity entity, DPLToken token, int idx) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return (pR && (DPL_TOKEN_REF_MAP == pR->tokenType)) ? pR->getTokenByIndex(idx) : 0;
}

DPLEntity DPLData::getEntityByPath(DPLEntity root, int path... ) {
	DPLEntity e = root;
	DustProdLightRef *pR = NULL;

	va_list args;
	va_start(args, path);

	while (e) {
		int p = va_arg(args, int);

		if ( !p ) {
			break;
		}

		pR = DustProdLightStore::store->getRef(e, p);

		if ( pR && pR->getCount() ) {
			switch ( pR->tokenType ) {
				case DPL_TOKEN_REF_SINGLE:
				e = pR->getRef(0);
				break;
				case DPL_TOKEN_REF_SET:
				case DPL_TOKEN_REF_ARR:
				p = va_arg(args, int);
				e = pR->getRef(p);
				break;
				case DPL_TOKEN_REF_MAP:
				p = va_arg(args, int);
				p = pR->getTokenByIndex(p);
				e = pR->getRef(p);
				break;
				default:
				e = 0;
				break;
			}
		} else {
			e = 0;
		}
	}

	va_end(args);

	return e;
}

void DPLData::visit(DPLEntity root, DPLVisitor *pVisitor, void *pHint) {
	DustProdLightEntity *pE = DustProdLightStore::store->getEntity(root);

	pVisitor->visitStart(root, pHint);
	pHint = pE->optVisit(pVisitor, 0, pHint);
	pVisitor->visitEnd(root, pHint);
}


DPLEntity DPLData::createEntity(DPLType primaryType) {
	return DustProdLightStore::store->createEntity(primaryType)->localId;
}

DPLType DPLData::getPrimaryType(DPLEntity entity) {
	return DustProdLightStore::store->getEntity(entity)->primaryType;
}
bool DPLData::hasType(DPLEntity entity, DPLType type) {
	return DustProdLightStore::store->getEntity(entity)->isOfType(type);
}
void DPLData::getAllTypes(DPLEntity entity, set<DPLType>& typeSet) {
	DustProdLightStore::store->getEntity(entity)->getAllTypes(typeSet);
}



string DPLUtils::getTypeName(DPLType type) {
	return DustProdLightStore::store->vecTypes[type];
}
string DPLUtils::getTokenName(DPLToken token) {
	DustProdLightToken *pT = DustProdLightStore::store->vecTokens[token];
	return pT->idStr;
}
