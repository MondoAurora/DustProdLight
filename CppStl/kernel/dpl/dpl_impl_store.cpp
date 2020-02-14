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
	getToken(notype, "", dpl_token_val_invalid);
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

void DPL::init() {
	if ( !DustProdLightStore::store) {
		DustProdLightStore::store = new DustProdLightStore();
	}
}
void DPL::shutdown() {
	if ( DustProdLightStore::store) {
		delete DustProdLightStore::store;
		DustProdLightStore::store = NULL;
	}
}


DPLType DPL::get_type(string typeName) {
	init();

	return DustProdLightStore::store->getType(typeName);
}

DPLToken DPL::get_token(DPLType type, string tokenName, DPLTokenType tokenType) {
	DustProdLightToken* pToken = DustProdLightStore::store->getToken(type, tokenName, tokenType);
	return pToken->id;
}

// int
void DPL::set_int(DPLEntity entity, DPLToken token, int i) {
	DustProdLightStore::store->setValue(entity, token, dpl_token_val_int, &i);
}

int DPL::get_int(DPLEntity entity, DPLToken token, int defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, dpl_token_val_int, token);
	return (0 == pVal) ? defValue : pVal->int_value;
}
// double
void DPL::set_double(DPLEntity entity, DPLToken token, double d) {
	DustProdLightStore::store->setValue(entity, token, dpl_token_val_double, &d);
}
double DPL::get_double(DPLEntity entity, DPLToken token, double defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, dpl_token_val_double, token);
	return (0 == pVal) ? defValue : pVal->double_value;
}
// string
void DPL::set_string(DPLEntity entity, DPLToken token, string s) {
	DustProdLightStore::store->setValue(entity, token, dpl_token_val_string, &s);

}
string DPL::get_string(DPLEntity entity, DPLToken token, string defValue) {
	DustProdLightValue *pVal = DustProdLightStore::store->getValue(entity, dpl_token_val_string, token);
	return (0 == pVal) ? defValue : pVal->string_value;
}

// references
bool DPL::set_ref(DPLEntity entity, DPLToken token, DPLEntity target, int key) {
	return DustProdLightStore::store->chgRef(dpl_chg_ref_set, entity, token, target, key);
}

int DPL::get_ref_count(DPLEntity entity, DPLToken token) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return pR ? pR->getCount() : 0;
}

DPLEntity DPL::get_ref(DPLEntity entity, DPLToken token, int key) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return pR ? pR->getRef(key) : 0;
}

DPLToken DPL::get_ref_key(DPLEntity entity, DPLToken token, int idx) {
	DustProdLightRef *pR = DustProdLightStore::store->getRef(entity, token);
	return (pR && (dpl_token_ref_map == pR->tokenType)) ? pR->getTokenByIndex(idx) : 0;
}

DPLEntity DPL::get_entity_by_path(DPLEntity root, int path... ) {
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
				case dpl_token_ref_single:
				e = pR->getRef(0);
				break;
				case dpl_token_ref_set:
				case dpl_token_ref_array:
				p = va_arg(args, int);
				e = pR->getRef(p);
				break;
				case dpl_token_ref_map:
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

		va_end(args);
	}

	return e;

}

DPLEntity DPL::create_entity(DPLType primaryType) {
	return DustProdLightStore::store->createEntity(primaryType)->localId;
}

DPLType DPL::get_primary_type(DPLEntity entity) {
	return DustProdLightStore::store->getEntity(entity)->primaryType;
}
bool DPL::has_type(DPLEntity entity, DPLType type) {
	return DustProdLightStore::store->getEntity(entity)->isOfType(type);
}
void DPL::get_all_types(DPLEntity entity, set<DPLType>& typeSet) {
	DustProdLightStore::store->getEntity(entity)->getTypes(typeSet);
}
