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

enum MontruBootItem {
	DPL_MBI_INVALID,

// first, attributes and references because they also need TokenType!
	DPL_MBI_ATT_ENTITY_GLOBALID,

	DPL_MBI_STORE_SOURCE,

	DPL_MBI_UNIT_MODEL,
	DPL_MBI_UNIT_IDEA,
	DPL_MBI_UNIT_NARRATIVE,
	DPL_MBI_UNIT_DIALOG,

	DPL_MBI_TYPE_MODEL_STORE,
	DPL_MBI_TYPE_MODEL_UNIT,
	DPL_MBI_TYPE_MODEL_ENTITY,

	DPL_MBI_TYPE_IDEA_TYPE,
	DPL_MBI_TYPE_IDEA_ATTRIBUTE,
	DPL_MBI_TYPE_IDEA_REFERENCE,


	DPL_MBI_, // end of list, USED in init cycle!
};

void DustProdLightStore::createBootEntity(DPLEntity entity, const char* name, DPLEntity primaryType, int owner, int hint){
	DustProdLightEntity *pe = &dataLocal[entity];

	pe->store = DPL_MBI_STORE_SOURCE;
	pe->localId = entity;

	switch ( primaryType ) {
	case DPL_MBI_INVALID:
		pe->tokenType = DPL_TOKEN_INVALID;
		break;
	case DPL_MBI_TYPE_MODEL_STORE:
		pe->tokenType = DPL_TOKEN_STORE;
		break;
	case DPL_MBI_TYPE_MODEL_UNIT:
		pe->tokenType = DPL_TOKEN_UNIT;
		break;
	case DPL_MBI_TYPE_IDEA_TYPE:
		pe->tokenType = DPL_TOKEN_TYPE;
		break;
	case DPL_MBI_TYPE_IDEA_ATTRIBUTE:
	case DPL_MBI_TYPE_IDEA_REFERENCE:
		pe->tokenType = (DPLTokenType) hint;
		break;
	default:
		pe->tokenType = DPL_TOKEN_ENTITY;
		break;
	}

	pe->primaryType = primaryType;
	pe->types.insert(primaryType);

	string globalId = getMetaEntityId(pe->tokenType, name, owner);
//	pe->values[DPL_MBI_ATT_ENTITY_GLOBALID].valStr = globalId;
	pe->values[DPL_MBI_ATT_ENTITY_GLOBALID].set(DPL_TOKEN_VAL_STRING, &globalId);

}

string DustProdLightStore::getMetaEntityId(DPLTokenType tokenType, string name, DPLEntity parent) {
	switch ( tokenType ) {
	case DPL_TOKEN_INVALID:
		return name;
	case DPL_TOKEN_STORE:
		return name + DPL_SEP_STORE;
	case DPL_TOKEN_UNIT:
		return dataLocal[DPL_MBI_STORE_SOURCE].values[DPL_MBI_ATT_ENTITY_GLOBALID].valStr + DPL_SEP_STORE + name;
	default:
		return parent ? dataLocal[parent].values[DPL_MBI_ATT_ENTITY_GLOBALID].valStr + DPL_SEP_ID + name : name;
	}

	return name;
}

//using namespace DplMetaCore;

DustProdLightStore* DustProdLightStore::store = NULL;

DustProdLightStore::DustProdLightStore() {
	nextEntityId = DPL_MBI_;

	createBootEntity(DPL_MBI_INVALID, "!!INVALID!!", DPL_MBI_INVALID);

	createBootEntity(DPL_MBI_STORE_SOURCE, "Source", DPL_MBI_TYPE_MODEL_STORE);

	createBootEntity(DPL_MBI_UNIT_MODEL, "Model", DPL_MBI_TYPE_MODEL_UNIT);
	createBootEntity(DPL_MBI_UNIT_IDEA, "Idea", DPL_MBI_TYPE_MODEL_UNIT);
	createBootEntity(DPL_MBI_UNIT_NARRATIVE, "Narrative", DPL_MBI_TYPE_MODEL_UNIT);
	createBootEntity(DPL_MBI_UNIT_DIALOG, "Dialog", DPL_MBI_TYPE_MODEL_UNIT);

	createBootEntity(DPL_MBI_TYPE_MODEL_STORE, "Store", DPL_MBI_TYPE_IDEA_TYPE, DPL_MBI_UNIT_MODEL);
	createBootEntity(DPL_MBI_TYPE_MODEL_UNIT, "Unit", DPL_MBI_TYPE_IDEA_TYPE, DPL_MBI_UNIT_MODEL);
	createBootEntity(DPL_MBI_TYPE_MODEL_ENTITY, "Entity", DPL_MBI_TYPE_IDEA_TYPE, DPL_MBI_UNIT_MODEL);

	createBootEntity(DPL_MBI_TYPE_IDEA_TYPE, "Type", DPL_MBI_TYPE_IDEA_TYPE, DPL_MBI_UNIT_IDEA);
	createBootEntity(DPL_MBI_TYPE_IDEA_ATTRIBUTE, "Attribute", DPL_MBI_TYPE_IDEA_TYPE, DPL_MBI_UNIT_IDEA);
	createBootEntity(DPL_MBI_TYPE_IDEA_REFERENCE, "Reference", DPL_MBI_TYPE_IDEA_TYPE, DPL_MBI_UNIT_IDEA);

	createBootEntity(DPL_MBI_ATT_ENTITY_GLOBALID, "GlobalId", DPL_MBI_TYPE_IDEA_ATTRIBUTE, DPL_MBI_TYPE_MODEL_ENTITY, DPL_TOKEN_VAL_STRING);

	cout << "DustProdLightStore - Initialized" << endl;
}

DustProdLightStore::~DustProdLightStore() {
	cout << "DustProdLightStore - Shutdown" << endl;
}

DPLEntity DustProdLightStore::getMetaEntity(DPLTokenType tokenType, string name, DPLEntity parent) {
	string id = getMetaEntityId(tokenType, name, parent);
	int localId = dataGlobal[id];

	if ( localId ) {
		return localId;
	} else {
		localId = ++ nextEntityId;

		dataGlobal[id] = localId;
		DustProdLightEntity *pe = &dataLocal[localId];

		pe->localId = localId;
		pe->tokenType = tokenType;

		switch ( tokenType ) {
		case DPL_TOKEN_INVALID:
			pe->primaryType = DPL_MBI_INVALID;
			break;
		case DPL_TOKEN_STORE:
			pe->primaryType = DPL_MBI_TYPE_MODEL_STORE;
			break;
		case DPL_TOKEN_UNIT:
			pe->primaryType = DPL_MBI_TYPE_MODEL_UNIT;
			break;
		case DPL_TOKEN_TYPE:
			pe->primaryType = DPL_MBI_TYPE_IDEA_TYPE;
			break;
		case DPL_TOKEN_VAL_BOOL:
		case DPL_TOKEN_VAL_DOUBLE:
		case DPL_TOKEN_VAL_INT:
		case DPL_TOKEN_VAL_STRING:
			pe->primaryType = DPL_MBI_TYPE_IDEA_ATTRIBUTE;
			break;
		case DPL_TOKEN_REF_SINGLE:
		case DPL_TOKEN_REF_SET:
		case DPL_TOKEN_REF_ARR:
		case DPL_TOKEN_REF_MAP:
			pe->primaryType = DPL_MBI_TYPE_IDEA_REFERENCE;
			break;
		default:
			pe->primaryType = DPL_TOKEN_INVALID;
			break;
		}

		pe->types.insert(pe->primaryType);
		pe->values[DPL_MBI_ATT_ENTITY_GLOBALID].set(DPL_TOKEN_VAL_STRING, &id);
//		pe->values[DPL_MBI_ATT_ENTITY_GLOBALID].valStr = id;
	}

	return localId;
}


DustProdLightEntity *DustProdLightStore::validateGetToken(DPLEntity token, DPLTokenType tokenType) {
	DustProdLightEntity *pToken = &dataLocal[token];

	if (pToken->tokenType != tokenType) {
		throw DPLErrInvalidValueType();
	}

	return pToken;
}


DustProdLightValue *DustProdLightStore::getValue(DPLEntity entity, DPLTokenType tokenType, DPLEntity token) {
	store->validateGetToken(token, tokenType);
	DustProdLightEntity *pEntity = &store->dataLocal[entity];
	return (pEntity->values.end() == pEntity->values.find(token)) ? 0 : &pEntity->values[token];
}

void DustProdLightStore::setValue(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void* pVal) {
	validateGetToken(token, tokenType);
	DustProdLightEntity *pEntity = &store->dataLocal[entity];
	pEntity->values[token].set(tokenType, pVal);
}

bool DustProdLightStore::chgRef(DPLChange chg, DPLEntity entity, DPLEntity token, DPLEntity target, int key) {
	bool ret = false;

	DustProdLightEntity *pEntity = &dataLocal[entity];
	DustProdLightRef *pRef = pEntity->refs[token];

	if (pRef) {
		if (!pRef->getBy(target, key)) {
			new DustProdLightRef(pRef, target, key);
		}
	} else {
		DustProdLightEntity *pToken = &dataLocal[token];
		pEntity->refs[token] = new DustProdLightRef(pToken, entity, target, key);
	}

	return ret;
}

DustProdLightRef* DustProdLightStore::getRef(DPLEntity entity, DPLEntity token) {
	DustProdLightEntity *pEntity = &store->dataLocal[entity];
	DustProdLightRef *pRef = pEntity->refs[token];

	return pRef;
}

DustProdLightEntity *DustProdLightStore::createEntity(DPLEntity primaryType) {
	DPLEntity ret = nextEntityId;
	DustProdLightEntity *pEntity = &dataLocal[nextEntityId++];
	pEntity->localId = ret;
	pEntity->primaryType = primaryType;
	pEntity->tokenType = DPL_TOKEN_ENTITY;
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


DPLEntity DPLMeta::getUnit(string unitName) {
	init();

	return DustProdLightStore::store->getMetaEntity(DPL_TOKEN_UNIT, unitName, 0);
}

DPLEntity DPLMeta::getType(DPLEntity unit, string typeName) {
	return DustProdLightStore::store->getMetaEntity(DPL_TOKEN_TYPE, typeName, unit);
}

DPLEntity DPLMeta::getToken(DPLEntity type, string tokenName, DPLTokenType tokenType) {
	return DustProdLightStore::store->getMetaEntity(tokenType, tokenName, type);
}

DPLEntity DPLMeta::getToken(string tokenId) {
	map<string, int>::iterator i = DustProdLightStore::store->dataGlobal.find(tokenId);
	return (DustProdLightStore::store->dataGlobal.end() == i) ? 0 : i->second;
}

void DPLData::setBool(DPLEntity entity, DPLEntity token, bool b) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_BOOL, &b);
}

void DPLData::setInt(DPLEntity entity, DPLEntity token, int i) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_INT, &i);
}

bool DPLData::getBool(DPLEntity entity, DPLEntity token, bool defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_BOOL, token);
	return (0 == pVal) ? defValue : pVal->valInt;
}

int DPLData::getInt(DPLEntity entity, DPLEntity token, int defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_INT, token);
	return (0 == pVal) ? defValue : pVal->valInt;
}

void DPLData::setDouble(DPLEntity entity, DPLEntity token, double d) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_DOUBLE, &d);
}
double DPLData::getDouble(DPLEntity entity, DPLEntity token, double defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_DOUBLE, token);
	return (0 == pVal) ? defValue : pVal->valDbl;
}

void DPLData::setString(DPLEntity entity, DPLEntity token, string s) {
	DustProdLightStore::store->setValue(entity, token, DPL_TOKEN_VAL_STRING, &s);

}
string DPLData::getString(DPLEntity entity, DPLEntity token, string defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, DPL_TOKEN_VAL_STRING, token);
	return (0 == pVal) ? defValue : pVal->valStr;
}

// references
bool DPLData::setRef(DPLEntity entity, DPLEntity token, DPLEntity target, int key) {
	return DustProdLightStore::store->chgRef(DPL_CHG_REF_SET, entity, token, target, key);
}

int DPLData::getRefCount(DPLEntity entity, DPLEntity token) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return pR ? pR->getCount() : 0;
}

DPLEntity DPLData::getRef(DPLEntity entity, DPLEntity token, int key) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return pR ? pR->getRef(key) : 0;
}

DPLEntity DPLData::getRefKey(DPLEntity entity, DPLEntity token, int idx) {
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


DPLEntity DPLData::createEntity(DPLEntity primaryType) {
	return DustProdLightStore::store->createEntity(primaryType)->localId;
}

DPLEntity DPLData::getPrimaryType(DPLEntity entity) {
	return DustProdLightStore::store->getEntity(entity)->primaryType;
}
bool DPLData::hasType(DPLEntity entity, DPLEntity type) {
	return DustProdLightStore::store->getEntity(entity)->isOfType(type);
}
void DPLData::getAllTypes(DPLEntity entity, set<DPLEntity>& typeSet) {
	DustProdLightStore::store->getEntity(entity)->getAllTypes(typeSet);
}



string DPLUtils::getGlobalId(DPLEntity entity) {
	return DPLData::getString(entity, DPL_MBI_ATT_ENTITY_GLOBALID, "???");
}
