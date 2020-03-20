/*
 * dpluproc.cpp
 *
 * Process implementations
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#include <vector>

#include <iostream>
#include "dpl_proc.h"
#include "../dplutils.h"

using namespace std;

/****************************
 *
 * DustProdLightAgent
 *
 ****************************/

DustProdLightAgent::DustProdLightAgent() :
		pDialog(NULL), result(DPL_PROCESS_ACCEPT) {
}
DustProdLightAgent::~DustProdLightAgent() {

}

DPLProcessResult DustProdLightAgent::dplProcess() {
	return DPL_PROCESS_ACCEPT;
}

void DustProdLightAgent::init(DPLEntity eAgentDef, DustProdLightDialog *pDialog_) {
}
void DustProdLightAgent::step() {
}
void DustProdLightAgent::stepUp() {
}
void DustProdLightAgent::finish(bool error) {
}

/****************************
 *
 * DustProdLightDialog
 *
 ****************************/

DustProdLightDialog::DustProdLightDialog() {
}

DustProdLightDialog::~DustProdLightDialog() {

}

DPLProcessResult DustProdLightDialog::dplProcess() {
	return DPL_PROCESS_ACCEPT;
}

/****************************
 *
 * Runtime
 *
 ****************************/
DustProdLightRuntime *DustProdLightRuntime::pRuntime = NULL;

//map<DPLEntity, const DPLModule*> DustProdLightRuntime::logicFactory;
//map<string, int> DustProdLightRuntime::dataGlobal;
//
//DustProdLightProcess* DustProdLightRuntime::pProcessMain = NULL;
//DustProdLightThread* DustProdLightRuntime::pThreadActive = NULL;
//
//int DustProdLightRuntime::nextEntityId = DPL_MBI_;

DustProdLightRuntime::DustProdLightRuntime() {
	nextEntityId = DPL_MBI_;

	pProcessMain = new DustProdLightProcess(NULL);
	pThreadActive = new DustProdLightThread(pProcessMain);

}

DustProdLightRuntime::~DustProdLightRuntime() {
	delete pProcessMain;
	delete pThreadActive;
}


void DustProdLightRuntime::init() {
	if (!pRuntime) {
		pRuntime = new DustProdLightRuntime();
		DPLMain::createBootEntities();
	}
}

void DustProdLightRuntime::release() {
	if (pRuntime) {
		delete pRuntime;
		pRuntime = NULL;
	}
}

DustProdLightEntity* DustProdLightRuntime::resolveEntity(DPLEntity entity) {
	return &pProcessMain->emapHeap[entity];
}

void DustProdLightRuntime::validateToken(DPLEntity token, DPLTokenType tokenType) {
	DustProdLightEntity *pToken = &pProcessMain->emapHeap[token];
	if (pToken->tokenType != tokenType) {
		throw DPLError(DPL_ERR_UNSPECIFIED);
	}
}

DustProdLightValue *DustProdLightRuntime::getValue(DPLEntity entity, DPLTokenType tokenType, DPLEntity token) {
	validateToken(token, tokenType);
	DustProdLightEntity *pEntity = resolveEntity(entity);
	return (pEntity->values.end() == pEntity->values.find(token)) ? 0 : &pEntity->values[token];
}

void DustProdLightRuntime::setValue(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void* pVal) {
	validateToken(token, tokenType);
	DustProdLightEntity *pEntity = resolveEntity(entity);
	pEntity->values[token].set(tokenType, pVal);
}


bool DustProdLightRuntime::chgRef(DPLChange chg, DPLEntity entity, DPLEntity token, DPLEntity target, int key) {
	bool ret = false;

	DustProdLightEntity *pEntity = resolveEntity(entity);
	DustProdLightRef *pRef = mapOptGet(pEntity->refs, token);

	if (pRef) {
		if (!pRef->getBy(target, key)) {
			new DustProdLightRef(pRef, target, key);
		}
	} else {
		DustProdLightEntity *pToken = &pProcessMain->emapHeap[token];
		pEntity->refs[token] = new DustProdLightRef(pToken, entity, target, key);
	}

	return ret;
}

DustProdLightRef* DustProdLightRuntime::getRef(DPLEntity entity, DPLEntity token) {
	DustProdLightEntity *pEntity = resolveEntity(entity);
	DustProdLightRef *pRef = mapOptGet(pEntity->refs, token);

	return pRef;
}

DustProdLightEntity *DustProdLightRuntime::createEntity(DPLEntity primaryType) {
	DPLEntity ret = nextEntityId;
	DustProdLightEntity *pEntity = &pProcessMain->emapHeap[nextEntityId++];

	pEntity->localId = ret;
	pEntity->primaryType = primaryType;
	pEntity->tokenType = DPL_TOKEN_ENTITY;

	return pEntity;
}
/*******************************
 *
 * DustProdLightImplementation
 *
 *******************************/

string DustProdLightRuntime::getMetaEntityId(DPLTokenType tokenType, string name, DPLEntity parent) {
	switch (tokenType) {
	case DPL_ENTITY_INVALID:
		return name;
	case DPL_TOKEN_STORE: {
		string str = name;
		return str + DPL_SEP_STORE;
	}
	case DPL_TOKEN_UNIT:
		return DustProdLightRuntime::pRuntime->pProcessMain->emapHeap[DPL_MBI_STORE_SOURCE].getString(DPL_MBI_ATT_ENTITY_GLOBALID)
				+ DPL_SEP_STORE + name;
	default:
		return
				parent ?
						DustProdLightRuntime::pRuntime->pProcessMain->emapHeap[parent].getString(DPL_MBI_ATT_ENTITY_GLOBALID) + DPL_SEP_ID
								+ name :
						name;
	}

	return name;
}

void DustProdLightRuntime::initMetaEntity(DPLEntity entity, DPLTokenType tokenType, string name, DPLEntity parent) {
	DustProdLightEntity* pE = &DustProdLightRuntime::pRuntime->pProcessMain->emapHeap[entity];
	pE->initMetaEntity(entity, tokenType, name, parent);
}
/****************************
 *
 * DPL API
 *
 ****************************/


void DPLData::setBool(DPLEntity entity, DPLEntity token, bool b) {
	DustProdLightRuntime::pRuntime->setValue(entity, token, DPL_TOKEN_VAL_BOOL, &b);
}

void DPLData::setInt(DPLEntity entity, DPLEntity token, int i) {
	DustProdLightRuntime::pRuntime->setValue(entity, token, DPL_TOKEN_VAL_INT, &i);
}

bool DPLData::getBool(DPLEntity entity, DPLEntity token, bool defValue) {
	DustProdLightValue *pVal = DustProdLightRuntime::pRuntime->getValue(entity, DPL_TOKEN_VAL_BOOL, token);
	return (0 == pVal) ? defValue : pVal->valInt;
}

int DPLData::getInt(DPLEntity entity, DPLEntity token, int defValue) {
	DustProdLightValue *pVal = DustProdLightRuntime::pRuntime->getValue(entity, DPL_TOKEN_VAL_INT, token);
	return (0 == pVal) ? defValue : pVal->valInt;
}

void DPLData::setDouble(DPLEntity entity, DPLEntity token, double d) {
	DustProdLightRuntime::pRuntime->setValue(entity, token, DPL_TOKEN_VAL_DOUBLE, &d);
}
double DPLData::getDouble(DPLEntity entity, DPLEntity token, double defValue) {
	DustProdLightValue *pVal = DustProdLightRuntime::pRuntime->getValue(entity, DPL_TOKEN_VAL_DOUBLE, token);
	return (0 == pVal) ? defValue : pVal->valDbl;
}

void DPLData::setString(DPLEntity entity, DPLEntity token, string s) {
	DustProdLightRuntime::pRuntime->setValue(entity, token, DPL_TOKEN_VAL_STRING, &s);

}
string DPLData::getString(DPLEntity entity, DPLEntity token, string defValue) {
	DustProdLightValue *pVal = DustProdLightRuntime::pRuntime->getValue(entity, DPL_TOKEN_VAL_STRING, token);
	return (0 == pVal) ? defValue : pVal->valStr;
}

// references
bool DPLData::setRef(DPLEntity entity, DPLEntity token, DPLEntity target, int key) {
	return DustProdLightRuntime::pRuntime->chgRef(DPL_CHG_REF_SET, entity, token, target, key);
}

unsigned int DPLData::getRefCount(DPLEntity entity, DPLEntity token) {
	DustProdLightRef *pR = DustProdLightRuntime::pRuntime->getRef(entity, token);
	return pR ? pR->getCount() : 0;
}

DPLEntity DPLData::getRef(DPLEntity entity, DPLEntity token, int key) {
	DustProdLightRef *pR = DustProdLightRuntime::pRuntime->getRef(entity, token);
	return pR ? pR->getRef(key) : 0;
}

DPLEntity DPLData::getRefKey(DPLEntity entity, DPLEntity token, int idx) {
	DustProdLightRef *pR = DustProdLightRuntime::pRuntime->getRef(entity, token);
	return (pR && (DPL_TOKEN_REF_MAP == pR->tokenType)) ? pR->getTokenByIndex(idx) : 0;
}

DPLEntity DPLData::getEntityByPath(DPLContext ctx, ...) {
	DPLEntity e = ctx;
//	DPLEntity e = DustProdLightImplementation::optResolveContext(ctx);

	DustProdLightRef *pR = NULL;

	va_list args;
	va_start(args, ctx);

	while (e) {
		int p = va_arg(args, int);

		if (!p) {
			break;
		}

		pR = DustProdLightRuntime::pRuntime->getRef(e, p);

		if (pR && pR->getCount()) {
			switch (pR->tokenType) {
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
	DustProdLightEntity *pE = DustProdLightRuntime::pRuntime->resolveEntity(root);

	pVisitor->visitStart(root, pHint);
	pHint = pE->optVisit(pVisitor, 0, pHint);
	pVisitor->visitEnd(root, pHint);
}

DPLEntity DPLData::createEntity(DPLEntity primaryType) {
	return DustProdLightRuntime::pRuntime->createEntity(primaryType)->localId;
}

DPLEntity DPLData::getPrimaryType(DPLEntity entity) {
	return DustProdLightRuntime::pRuntime->resolveEntity(entity)->primaryType;
}
bool DPLData::hasType(DPLEntity entity, DPLEntity type) {
	return DustProdLightRuntime::pRuntime->resolveEntity(entity)->isOfType(type);
}
void DPLData::getAllTypes(DPLEntity entity, set<DPLEntity>& typeSet) {
	DustProdLightRuntime::pRuntime->resolveEntity(entity)->getAllTypes(typeSet);
}

string DPLUtils::getGlobalId(DPLEntity entity) {
	return DPLData::getString(entity, DPL_MBI_ATT_ENTITY_GLOBALID, "???");
}

DPLEntity DPLData::getEntityById(string globalId) {
	map<string, int>::iterator i = DustProdLightRuntime::pRuntime->dataGlobal.find(globalId);
	return (DustProdLightRuntime::pRuntime->dataGlobal.end() == i) ? 0 : i->second;
}

DPLEntity DPLData::getMetaEntity(DPLTokenType tokenType, string name, DPLEntity parent) {
	DustProdLightRuntime::pRuntime->init();

	string id = DustProdLightRuntime::getMetaEntityId(tokenType, name, parent);
	int entity = DustProdLightRuntime::pRuntime->dataGlobal[id];

	if (!entity) {
		entity = ++DustProdLightRuntime::pRuntime->nextEntityId;

		DustProdLightRuntime::pRuntime->dataGlobal[id] = entity;
		DustProdLightEntity *pe = &DustProdLightRuntime::pRuntime->pProcessMain->emapHeap[entity];

		pe->initMetaEntity(entity, tokenType, name, parent);
	}

	return entity;
}

DPLProcessResult DPLMain::send(DPLEntity target, DPLEntity command, DPLEntity param) {
	return DPL_PROCESS_REJECT;
}

void DPLMain::registerLogicProvider(const DPLModule *pLogicFactory, ...) {

	va_list args;
	va_start(args, pLogicFactory);
	int mId;

	while (DPL_ENTITY_INVALID != (mId = va_arg(args, int))) {
		DustProdLightRuntime::pRuntime->logicFactory[mId] = pLogicFactory;
	}

	va_end(args);
}
