#include "dpl_stl.h"

#include <vector>
#include <set>

#include <iostream>
#include <dplutils.h>

#include <_dplgen_module_dpl_stl.h>

using namespace std;

using namespace DPLUnitNarrative;
using namespace DPLUnitNative;
using namespace DPLUnitDust;

DustProdLightRuntime *DustProdLightRuntime::pRuntime = NULL;

/*******************************
 *
 * Block
 *
 *******************************/

DustProdLightState::DustProdLightState() :
		pStore(NULL), pLogic(NULL) {
}

DustProdLightState::~DustProdLightState() {
}

DustProdLightEntity* DustProdLightState::getEntity(DPLEntity e) {
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

DPLProcessResult DustProdLightState::init(DustProdLightEntity *pTask, DustProdLightState *pParent) {
	DustProdLightRef *pRef;
	DustProdLightEntity *pSelf;

	if (AgentCall == pTask->primaryType) {
//		pRef = mapOptGet(pTask->refs, RefExecAtomParam);
//		emapRef[DPL_CTX_PARAM] = pRef ? DustProdLightRuntime::pRuntime->resolveEntity(pRef->target) : NULL;
//
//		pRef = mapOptGet(pTask->refs, RefExecAtomCommand);
//
//		pRef = mapOptGet(pTask->refs, RefExecAtomTarget);
//		pSelf = pRef ? DustProdLightRuntime::pRuntime->resolveEntity(pRef->target) : NULL;
	} else {
		pSelf = pTask; // maybe shallow copy?
	}

	pStore = pParent ? pParent->pStore : &DustProdLightRuntime::pRuntime->store;

	DPLEntity eRes = mapOptGetDef(DustProdLightRuntime::pRuntime->agentResolution, pSelf->primaryType, DPL_ENTITY_INVALID);
	if ( eRes ) {
		pSelf->primaryType = DustProdLightRuntime::pRuntime->resolveEntity(eRes)->primaryType;
	}

	emapRef[DPL_CTX_SELF] = pSelf;

	pLogic =  DustProdLightRuntime::createLogic(pSelf->primaryType);

	return DPL_PROCESS_ACCEPT;
}

DPLProcessResult DustProdLightState::dplActionExecute() {
	DPLProcessResult ret = pLogic->dplActionExecute();
	DustProdLightRuntime::getCurrentCore()->lastResult = ret;
	return ret;
}

DPLProcessResult DustProdLightState::dplResourceRelease() {
	DustProdLightEntity *pSelf = emapRef[DPL_CTX_SELF];

	pSelf->releaseActions();

	emapRef.clear();
	return DPL_PROCESS_ACCEPT;
}

/****************************
 *
 * Agent
 *
 ****************************/

DustProdLightPDA::DustProdLightPDA() {

}

DustProdLightPDA::~DustProdLightPDA() {
	dplResourceRelease();
}

DPLProcessResult DustProdLightPDA::dplActionExecute() {
	DPLProcessResult ret = stack[stackPos]->dplActionExecute();

	if (!DPLUtils::isReading(ret)) {
		if (0 < stackPos) {
			--stackPos;
			ret = DPL_PROCESS_READ;
		}
	}

	return ret;
}

DPLProcessResult DustProdLightPDA::dplResourceRelease() {
	stack.clear();
	stackPos = -1;
	return DPL_PROCESS_ACCEPT;
}

void DustProdLightPDA::relayEntry(DustProdLightState *pStateRelay) {
	stack[++stackPos] = pStateRelay;
}

void DustProdLightPDA::relayExit() {
	stack[stackPos--] = NULL;
}

/****************************
 *
 * Dialog
 *
 ****************************/

DustProdLightDialog::DustProdLightDialog()
:pData(NULL)
{
	pdas.resize(1);
}

DustProdLightDialog::~DustProdLightDialog() {
	dplResourceRelease();
}

DPLProcessResult DustProdLightDialog::dplActionExecute() {
	DPLProcessResult ret = pdas[currPDAIdx].dplActionExecute();

	switch (ret) {
	case DPL_PROCESS_ACCEPT:
		if (currPDAIdx) {
			currPDAIdx = 0;
			ret = DPL_PROCESS_READ;
		}
		break;
	case DPL_PROCESS_ACCEPT_PASS:
		if (++currPDAIdx == pdas.size()) {
			currPDAIdx = 0;
		}
		ret = DPL_PROCESS_READ;
		break;
	default:
		// do nothing
		break;
	}

	return ret;
}

DPLProcessResult DustProdLightDialog::dplResourceRelease() {
	pdas.clear();
	currPDAIdx = 0;
	return DPL_PROCESS_ACCEPT;
}

/****************************
 *
 * Core
 *
 ****************************/

DPLProcessResult DustProdLightCore::dplActionExecute() {
	while (pDialog) {
		DPLProcessResult dlgRet = pDialog->dplActionExecute();
		if (!DPLUtils::isReading(dlgRet) ) {
			pDialog = NULL;
		}

		DustProdLightRuntime::pRuntime->pScheduler->dplActionExecute();
	}

	return lastResult;
}

DustProdLightEntity* DustProdLightCore::resolveEntity(DPLEntity entity) {
	return pDialog ? pDialog->getCurrentPda()->resolveEntity(entity) : NULL;
}

DPLProcessResult DustProdLightCore::run(int dlgIdx, DustProdLightState *pState, int agentCount) {
	pDialog = &DustProdLightRuntime::pRuntime->dialogs[dlgIdx];

	for ( int i = 0; i < agentCount; ++i) {
		pDialog->getPda(i)->relayEntry(&pState[i]);
	}

	return dplActionExecute();
}

/****************************
 *
 * Runtime
 *
 ****************************/

DustProdLightRuntime::DustProdLightRuntime() :
		pScheduler(DPLUtils::getNullLogic()) {

	cores.resize(1);
	dialogs.resize(1);
}

DustProdLightRuntime::~DustProdLightRuntime() {
}

DustProdLightCore* DustProdLightRuntime::getCurrentCore() {
	return &pRuntime->cores[0];
}

void DustProdLightRuntime::init() {
	if (!pRuntime) {
		pRuntime = new DustProdLightRuntime();
//		DPLMain::createBootEntities();
	}
}

void DustProdLightRuntime::release() {
	if (pRuntime) {
		delete pRuntime;
		pRuntime = NULL;
	}
}

DustProdLightEntity *DustProdLightRuntime::getRootEntity(DPLEntity entity) {
	return pRuntime->store.resolve(entity);
}

DPLNativeLogic *DustProdLightRuntime::createLogic(DPLEntity eAction) {
	DPLNativeLogic *pLogic = NULL;

	DPLModule *pModule = pRuntime->logicFactory[eAction];
	if (pModule) {
		pLogic = pModule->createLogic(eAction);
		if (pLogic) {
			pLogic->dplResourceInit();
		}
	}

	return pLogic;
}

void DustProdLightRuntime::releaseLogic(DPLEntity eAction, DPLNativeLogic *pAction) {
	DPLModule *pMod = pRuntime->logicFactory[eAction];
	pMod->releaseLogic(eAction, pAction);
}

DustProdLightEntity* DustProdLightRuntime::resolveEntity(DPLEntity entity) {
	DustProdLightEntity *pE = getCurrentCore()->resolveEntity(entity);

	return pE ? pE : getRootEntity(entity);
}

void DustProdLightRuntime::validateToken(DPLEntity token, DPLTokenType tokenType) {
	DustProdLightEntity *pToken = getRootEntity(token);
	if (pToken->primaryType != tokenType) {
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
	DustProdLightEntity *pEntity = getRootEntity(DPL_ENTITY_APPEND);
	pEntity->primaryType = primaryType;
	return pEntity;
}

string DustProdLightRuntime::getMetaEntityId(DPLEntity primaryType, string name, DPLEntity parent) {
	switch (primaryType) {
	case DPL_ENTITY_INVALID:
		return name;
	case DPL_MBI_TYPE_MODEL_STORE: {
		string str = name;
		return str + DPL_SEP_STORE;
	}
	case DPL_TOKEN_UNIT:
		return getRootEntity(DPL_MBI_STORE_SOURCE)->getString(DPL_MBI_ATT_ENTITY_GLOBALID) + DPL_SEP_STORE + name;
	case DPL_MBI_TYPE_NATIVE_MODULE: {
		string str = "Module" ;
		return str + DPL_SEP_STORE + name;
	}
	default:
		return parent ? getRootEntity(parent)->getString(DPL_MBI_ATT_ENTITY_GLOBALID) + DPL_SEP_ID + name : name;
	}

	return name;
}

DPLEntity DustProdLightRuntime::initMetaEntity(int entity, DPLEntity primaryType, string name, DPLEntity parent) {
	DustProdLightEntity* pE = getRootEntity(entity);
	pE->initMetaEntity(entity, primaryType, name, parent);
	return entity;
}
/****************************
 *
 * DPL API
 *
 ****************************/

void DPLData::setBool(DPLEntity entity, DPLEntity token, bool b) {
	int val = b ? 1 : 0;
	DustProdLightRuntime::pRuntime->setValue(entity, token, DPL_TOKEN_VAL_BOOL, &val);
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

	return DPL_ENTITY_INVALID;
}

DPLEntity DPLData::getRefKey(DPLEntity entity, DPLEntity token, int idx) {
	DustProdLightRef *pR = DustProdLightRuntime::pRuntime->getRef(entity, token);
	return (pR && (DPL_TOKEN_REF_MAP == pR->tokenType)) ? pR->getTokenByIndex(idx) : 0;
}

DPLEntity DPLData::getEntityByPath(DPLEntity ctx, ...) {
	DPLEntity e = ctx;

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

DPLEntity DPLData::getMetaEntity(DPLEntity primaryType, string name, DPLEntity parent) {
	DustProdLightRuntime::init();

	string id = DustProdLightRuntime::getMetaEntityId(primaryType, name, parent);
	int entity = DustProdLightRuntime::pRuntime->dataGlobal[id];

	if (!entity) {
		DustProdLightEntity *pe = DustProdLightRuntime::getRootEntity(DPL_ENTITY_APPEND);

		entity = pe->localId;
		DustProdLightRuntime::pRuntime->dataGlobal[id] = entity;
		pe->initMetaEntity(entity, primaryType, name, parent);
	}

	return entity;
}

void DPLMain::registerModule(const char* moduleName, DPLModule *pModule) {
	DPLEntity eModule = DPLData::getMetaEntity(DPLUnitNative::TypeModule, moduleName);
	pModule->init(eModule);

	for ( int i = DPLData::getRefCount(eModule, DPLUnitNative::RefModuleAgents); i-->0; ) {
		int aId = DPLData::getRef(eModule, DPLUnitNative::RefModuleAgents, i);
		DustProdLightRuntime::pRuntime->logicFactory[aId] = pModule;
	}

	for ( int i = DPLData::getRefCount(eModule, DPLUnitNative::RefModuleAlgorithms); i-->0; ) {
		int aId = DPLData::getRefKey(eModule, DPLUnitNative::RefModuleAlgorithms, i);
		int rId = DPLData::getRef(eModule, DPLUnitNative::RefModuleAlgorithms, aId);
		DustProdLightRuntime::pRuntime->agentResolution[aId] = rId;
	}
}

DPLProcessResult DPLMain::run() {
	DPLEntity eProc = DPLData::getRef(AgentRuntime, DPLUnitDust::RefRuntimeMain);

	if (eProc) {
		DustProdLightEntity *pMainTask = DustProdLightRuntime::pRuntime->resolveEntity(eProc);
		DustProdLightCore *pCore = DustProdLightRuntime::getCurrentCore();
		bool dlg = AgentDialog == pMainTask->primaryType;
		int ac = dlg ? DPLData::getRefCount(eProc, DPLUnitTools::RefCollectionMembers) : 1;

		DustProdLightState states[ac];

		if (dlg) {
			for ( int i = 0; i < ac; ++i ) {
				DPLEntity eChildTask = DPLData::getRef(eProc, DPLUnitTools::RefCollectionMembers, i);
				DustProdLightEntity *pChildTask = DustProdLightRuntime::pRuntime->resolveEntity(eChildTask);
				states[i].init(pChildTask, NULL);
				states[i].emapRef[AgentDialog] = pMainTask;
			}
		} else {
			states[0].init(pMainTask, NULL);
		}

		return pCore->run(0, states, ac);
	} else {
		return DPL_PROCESS_ACCEPT_PASS;
	}
}
