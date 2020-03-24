/*
 * dpluproc.cpp
 *
 * Process implementations
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#include <vector>
#include <set>

#include <iostream>
#include "dpl_proc_runtime.h"
#include "../dplutils.h"

using namespace std;

/*******************************
 *
 * DustProdLightBlock
 *
 *******************************/

DPLEntity DustProdLightBlock::getMsgEntity(DPLEntity cmd, DPLEntity target) {
	DPLEntity eMsg = -(emapMsg.size() + 1);

	DustProdLightEntity* pMsg = &emapMsg[eMsg];

	pMsg->localId = eMsg;

	pMsg->chgRef(DPL_CHG_REF_SET, DustProdLightRuntime::pRefMsgCmd, cmd, 0);
	pMsg->chgRef(DPL_CHG_REF_SET, DustProdLightRuntime::pRefMsgTarget, target, 0);

	return eMsg;
}

DustProdLightEntity* DustProdLightBlock::getEntity(DPLEntity e) {
	if ( e < 0 ) {
		return &emapMsg[e];
	}

	EntityIterator i = emapLocal.find(e);
	DustProdLightEntity* ret;

	if (i == emapLocal.end()) {
		EntityPtrIterator pi = emapRef.find(e);
		ret = (pi == emapRef.end()) ? NULL : pi->second;
	} else {
		ret = &i->second;
	}

	return ret;
}

void DustProdLightBlock::init(DustProdLightEntity *pmsg) {
	emapRef[DPL_CTX_COMMAND] = DustProdLightRuntime::pRuntime->resolveEntity(pmsg->refs[DPL_MBI_REF_MESSAGE_COMMAND]->target);
	emapRef[DPL_CTX_SELF] = DustProdLightRuntime::pRuntime->resolveEntity(pmsg->refs[DPL_MBI_REF_MESSAGE_TARGET]->target);
	emapRef[DPL_CTX_PARAM] = pmsg;
}

DPLProcessResult DustProdLightBlock::exec(DPLEntity cmd) {
	DPLAction* pAction = actionByCmd[cmd];

	if ( ! pAction ) {
		DustProdLightEntity *pSelf = emapRef[DPL_CTX_SELF];
		pAction = pSelf->getActionByCommand(cmd);
		actionByCmd[cmd] = pAction;
	}

	return pAction->dplProcess();
}

void DustProdLightBlock::release() {
	DustProdLightEntity *pSelf = emapRef[DPL_CTX_SELF];

	pSelf->releaseActions();

	actionByCmd.clear();
	emapLocal.clear();
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
	DustProdLightBlock* b = getCurrBlock();

	int end = - (b->emapMsg.size() + 1);

	DustProdLightBlock *action = &stack[stackPos+1];

	for ( int mi = -1; mi > end; --mi ) {
		DustProdLightEntity *pmsg = &b->emapMsg[mi];
		action->init(pmsg);
		int cmd = pmsg->refs[DPL_MBI_REF_MESSAGE_COMMAND]->target;
		++stackPos;
		action->exec(cmd);
		--stackPos;
		action->release();
	}

	--stackPos;

	return DPL_PROCESS_ACCEPT;
}

DustProdLightBlock* DustProdLightAgent::init(DPLEntity eAgentStart, map<int, DustProdLightEntity> *pHeap_) {
	this->pHeap = pHeap_;
	stackPos = 0;

	return &stack[0];
}

void DustProdLightAgent::step() {
}
void DustProdLightAgent::stepUp() {
}
void DustProdLightAgent::finish(bool error) {
}

/****************************
 *
 * Runtime
 *
 ****************************/
DustProdLightRuntime *DustProdLightRuntime::pRuntime = NULL;
DustProdLightEntity *DustProdLightRuntime::pRefMsgCmd = NULL;
DustProdLightEntity *DustProdLightRuntime::pRefMsgTarget = NULL;

DustProdLightRuntime::DustProdLightRuntime() {
	nextEntityId = DPL_MBI_;

	threadSingle.pAgent = &agentMain;
	pThreadActive = &threadSingle;

}

DustProdLightRuntime::~DustProdLightRuntime() {
}

void DustProdLightRuntime::init() {
	if (!pRuntime) {
		pRuntime = new DustProdLightRuntime();
		DPLMain::createBootEntities();

		pRefMsgCmd = getRootEntity(DPL_MBI_REF_MESSAGE_COMMAND);
		pRefMsgTarget = getRootEntity(DPL_MBI_REF_MESSAGE_TARGET);
	}
}

void DustProdLightRuntime::release() {
	if (pRuntime) {
		delete pRuntime;
		pRuntime = NULL;
	}
}

DustProdLightEntity *DustProdLightRuntime::getRootEntity(DPLEntity entity) {
	return &pRuntime->agentMain.stack[0].emapLocal[entity];
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

	if ( pThreadActive->pAgent ) {
		pE = pThreadActive->pAgent->getCurrBlock()->getEntity(entity);
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
	DPLEntity ret = nextEntityId;
	DustProdLightEntity *pEntity = getRootEntity(nextEntityId++);

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

	DustProdLightEntity *pToken = DustProdLightRuntime::pRuntime->resolveEntity(token);
	if (DPL_TOKEN_COMMAND == pToken->tokenType) {
		return DustProdLightRuntime::pRuntime->pThreadActive->pAgent->getCurrBlock()->getMsgEntity(token, entity);
	}

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
	DustProdLightRuntime::pRuntime->init();

	string id = DustProdLightRuntime::getMetaEntityId(tokenType, name, parent);
	int entity = DustProdLightRuntime::pRuntime->dataGlobal[id];

	if (!entity) {
		entity = ++DustProdLightRuntime::pRuntime->nextEntityId;

		DustProdLightRuntime::pRuntime->dataGlobal[id] = entity;
		DustProdLightEntity *pe = DustProdLightRuntime::getRootEntity(entity);

		pe->initMetaEntity(entity, tokenType, name, parent);
	}

	return entity;
}

void DPLMain::signal(DPLSignal signal) {
	DustProdLightRuntime::pRuntime->pThreadActive->pAgent->dplProcess();
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
