/*
 * dpluproc.cpp
 *
 * Process implementations
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_stl.h"

#include <vector>
#include <set>

#include <iostream>
#include <dplutils.h>

#include <_dplgen_module_dpl_stl.h>

using namespace std;

DustProdLightRuntime *DustProdLightRuntime::pRuntime = NULL;

DustProdLightEntity *pRefMsgCmd = NULL;
DustProdLightEntity *pRefMsgTarget = NULL;


/*******************************
 *
 * DustProdLightBlock
 *
 *******************************/

DustProdLightEntity* DustProdLightBlock::getEntity(DPLEntity e) {
	EntityIterator i = pStore->emapLocal.find(e);
	DustProdLightEntity* ret;

	if (i == pStore->emapLocal.end()) {
		EntityPtrIterator pi = emapRef.find(e);
		ret = (pi == emapRef.end()) ? NULL : pi->second;
	} else {
		ret = &i->second;
	}

	return ret;
}

void DustProdLightBlock::init(DustProdLightEntity *pTask, DustProdLightBlock *pParent) {
	pStore = pParent ? pParent->pStore : &DustProdLightRuntime::pRuntime->store;
	emapRef[DPL_CTX_COMMAND] = DustProdLightRuntime::pRuntime->resolveEntity(pTask->refs[DPL_MBI_REF_TASK_COMMAND]->target);
	emapRef[DPL_CTX_SELF] = DustProdLightRuntime::pRuntime->resolveEntity(pTask->refs[DPL_MBI_REF_TASK_TARGET]->target);
	emapRef[DPL_CTX_PARAM] = pTask;
	pAction = NULL;
}

DPLProcessResult DustProdLightBlock::dplProcess() {
	if (!pAction) {
		DustProdLightEntity *pCmd = emapRef[DPL_CTX_COMMAND];
		DPLEntity cmd = pCmd ? pCmd->localId : DPL_MBI_CMD_PROCESS;
		DustProdLightEntity *pSelf = emapRef[DPL_CTX_SELF];
		pAction = pSelf->getActionByCommand(cmd);
	}

	return pAction->dplProcess();
}

void DustProdLightBlock::release() {
	DustProdLightEntity *pSelf = emapRef[DPL_CTX_SELF];

	pSelf->releaseActions();

	emapRef.clear();
}

/****************************
 *
 * DustProdLightAgent
 *
 ****************************/

DustProdLightAgent::DustProdLightAgent() {

}

DustProdLightAgent::~DustProdLightAgent() {

}

DPLProcessResult DustProdLightAgent::dplProcess() {
	DustProdLightCore *pC = DustProdLightCore::getCurrentCore();

	if ( DPL_PROCESS_ACCEPT != pC->lastResult ) {
		if ( 0 < stackPos ) {
			pC->pBlock = &stack[--stackPos];
			return DPL_PROCESS_ACCEPT;
		}
	}

	return pC->lastResult;
}

/****************************
 *
 * Core
 *
 ****************************/

DustProdLightCoreSingle DustProdLightCoreSingle::singleCore;

DustProdLightCore* DustProdLightCore::getCurrentCore() {
	return &DustProdLightCoreSingle::singleCore;
}

DPLProcessResult DustProdLightCore::dplProcess() {
	while ( pBlock ) {
		lastResult = pBlock->dplProcess();
		if ( DPL_PROCESS_ACCEPT != lastResult ) {
			pBlock = NULL;
		}

		DPLAction::optProcess(pAgent, lastResult);
		DPLAction::optProcess(pDialog, lastResult);
		DPLAction::optProcess(DustProdLightRuntime::pRuntime->pScheduler, lastResult);
	}

	return lastResult;
}
/****************************
 *
 * Runtime
 *
 ****************************/

DustProdLightRuntime::DustProdLightRuntime() {
	store.nextEntityId = DPL_MBI_;
}

DustProdLightRuntime::~DustProdLightRuntime() {
}

void DustProdLightRuntime::init() {
	if (!pRuntime) {
		pRuntime = new DustProdLightRuntime();
		DPLMain::createBootEntities();

		pRefMsgCmd = getRootEntity(DPL_MBI_REF_TASK_COMMAND);
		pRefMsgTarget = getRootEntity(DPL_MBI_REF_TASK_TARGET);
	}
}

void DustProdLightRuntime::release() {
	if (pRuntime) {
		delete pRuntime;
		pRuntime = NULL;
	}
}

DustProdLightEntity *DustProdLightRuntime::getRootEntity(DPLEntity entity) {
	return &pRuntime->store.emapLocal[entity];
}

DPLAction *DustProdLightRuntime::createAction(DPLEntity eAction) {
	DPLModule *pMod = pRuntime->logicFactory[eAction];
	return pMod ? pMod->createLogic(eAction) : NULL;
}

void DustProdLightRuntime::releaseAction(DPLEntity eAction, DPLAction *pAction) {
	DPLModule *pMod = pRuntime->logicFactory[eAction];
	pMod->releaseLogic(eAction, pAction);
}

DustProdLightEntity* DustProdLightRuntime::resolveEntity(DPLEntity entity) {
	DustProdLightEntity *pE = NULL;
	DustProdLightBlock *pB = DustProdLightCore::getCurrentCore()->pBlock;

	if ( pB ) {
		pE = pB->getEntity(entity);
	}

	return pE ? pE : getRootEntity(entity);
}

void DustProdLightRuntime::validateToken(DPLEntity token, DPLTokenType tokenType) {
	DustProdLightEntity *pToken = getRootEntity(token);
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
		DustProdLightEntity *pToken = getRootEntity(token);
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
	DPLEntity ret = store.nextEntityId;
	DustProdLightEntity *pEntity = getRootEntity(store.nextEntityId++);

	pEntity->localId = ret;
	pEntity->primaryType = primaryType;
	pEntity->tokenType = DPL_TOKEN_ENTITY;

	return pEntity;
}

string DustProdLightRuntime::getMetaEntityId(DPLTokenType tokenType, string name, DPLEntity parent) {
	switch (tokenType) {
	case DPL_ENTITY_INVALID:
		return name;
	case DPL_TOKEN_STORE: {
		string str = name;
		return str + DPL_SEP_STORE;
	}
	case DPL_TOKEN_UNIT:
		return getRootEntity(DPL_MBI_STORE_SOURCE)->getString(DPL_MBI_ATT_ENTITY_GLOBALID) + DPL_SEP_STORE + name;
	default:
		return parent ? getRootEntity(parent)->getString(DPL_MBI_ATT_ENTITY_GLOBALID) + DPL_SEP_ID + name : name;
	}

	return name;
}

void DustProdLightRuntime::initMetaEntity(DPLEntity entity, DPLTokenType tokenType, string name, DPLEntity parent) {
	DustProdLightEntity* pE = getRootEntity(entity);
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

	if (pR) {
		return pR->getRef(key);
	}

//	DustProdLightEntity *pToken = DustProdLightRuntime::pRuntime->resolveEntity(token);
//	if (DPL_TOKEN_COMMAND == pToken->tokenType) {
//		return DustProdLightRuntime::pRuntime->pThreadActive->pAgent->getCurrBlock()->getMsgEntity(token, entity);
//	}

	return DPL_ENTITY_INVALID;
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
	DustProdLightRuntime::init();

	string id = DustProdLightRuntime::getMetaEntityId(tokenType, name, parent);
	int entity = DustProdLightRuntime::pRuntime->dataGlobal[id];

	if (!entity) {
		entity = ++DustProdLightRuntime::pRuntime->store.nextEntityId;

		DustProdLightRuntime::pRuntime->dataGlobal[id] = entity;
		DustProdLightEntity *pe = DustProdLightRuntime::getRootEntity(entity);

		pe->initMetaEntity(entity, tokenType, name, parent);
	}

	return entity;
}

void DPLMain::registerLogicProvider(DPLModule *pLogicFactory, ...) {

	va_list args;
	va_start(args, pLogicFactory);
	int mId;

	while (DPL_ENTITY_INVALID != (mId = va_arg(args, int))) {
		DustProdLightRuntime::pRuntime->logicFactory[mId] = pLogicFactory;
	}

	va_end(args);
}

DPLProcessResult DPLMain::run() {
	DPLEntity eProc = DPLData::getRef(DPL_CTX_RUNTIME, DPLUnitDust::RefRuntimeMain);

	if (eProc) {
		DustProdLightEntity *pTask = DustProdLightRuntime::pRuntime->resolveEntity(eProc);

		DustProdLightBlock mainBlock;
		mainBlock.init(pTask, NULL);

		DustProdLightCore *pC = DustProdLightCore::getCurrentCore();
		pC->pBlock = &mainBlock;

		pC->dplProcess();

		return pC->lastResult;

	} else {
		return DPL_PROCESS_SUCCESS;
	}
}
