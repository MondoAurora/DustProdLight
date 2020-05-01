#include "data.h"
#include "proc_runtime.h"
#include "_dplgen_boot.h"
#include <_dplgen_module_dpl_stl.h>

#include <dplutils.h>

DustProdLightEntity::DustProdLightEntity() {
}

DustProdLightEntity::DustProdLightEntity(const DustProdLightEntity& e) :
		localId(e.localId), store(e.store), primaryType(e.primaryType), types(e.types),
		changed(e.changed), values(e.values) {
	for (EntityRefIterator iter = e.refs.begin(); iter != e.refs.end(); ++iter) {
		int k = iter->first;
		DustProdLightRef *pr = iter->second;
		refs[k] = new DustProdLightRef(*pr);
	}
}

DustProdLightEntity::~DustProdLightEntity() {
	if (pActionByAction) {
		releaseActions();
		delete pActionByAction;
	}
}

DustProdLightValue *DustProdLightEntity::getValue(DPLEntity token) {
	return (values.end() == values.find(token)) ? 0 : &values[token];
}

void DustProdLightEntity::setValue(DPLEntity token, DPLTokenType tokenType, void* pVal) {
	values[token].set(tokenType, pVal);
}

DPLEntity DustProdLightEntity::getRefEntity(DPLEntity token, int key) {
	DustProdLightRef *pRef = mapOptGet(refs, token);

	return pRef ? pRef->getRef(key) : DPL_ENTITY_INVALID;

}

bool DustProdLightEntity::chgRef(DPLChange chg, DustProdLightEntity *pToken, DPLEntity target, int key) {
	DustProdLightRef *pNewRef = NULL;

	DPLEntity token = pToken->localId;
	DustProdLightRef *pRef = mapOptGet(refs, token);

	if (pRef) {
		if (!pRef->getBy(target, key)) {
			pNewRef = new DustProdLightRef(pRef, target, key);
		}
	} else {
		refs[token] = pNewRef = new DustProdLightRef(pToken, localId, target, key);
	}

	if ( pNewRef ) {
		// special handling like Actions
	}

	return pNewRef;
}

DPLNativeLogic* DustProdLightEntity::getLogicByCommand(DPLEntity cmd) {
	DPLEntity eAction = NULL;
//	DPLEntity eAction = getRefEntity(DPLUnitModel::RefEntityActions, cmd);
	DPLNativeLogic* pAction = NULL;

	if ( pActionByAction ) {
		pAction = (*pActionByAction)[eAction];
	} else {
		pActionByAction = new map<DPLEntity, DPLNativeLogic*>();
	}

	if ( !pAction ) {
		pAction = DustProdLightRuntime::createLogic(eAction);
		(*pActionByAction)[eAction] = pAction;
	}

	return pAction;
}

void DustProdLightEntity::releaseActions() {
	if (pActionByAction) {
		for (ActionIterator ait = pActionByAction->begin(); ait != pActionByAction->end(); ++ait) {
			DPLEntity eAction = ait->first;
			DPLNativeLogic* pAction = ait->second;
			DPLModule *pMod = DustProdLightRuntime::pRuntime->logicFactory[eAction];
			pMod->releaseLogic(eAction, pAction);
		}

		pActionByAction->clear();
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

bool DustProdLightEntity::isOfType(DPLEntity type) {
	return types.end() != types.find(type);
}

void DustProdLightEntity::getAllTypes(set<DPLEntity>& typeSet) {
	typeSet.clear();
	typeSet.insert(types.begin(), types.end());
}

void* DustProdLightEntity::optVisit(DPLVisitor *pVisitor, int key, void *pHint) {
	void *pRet = pHint;
	DPLProcessResult fr = pVisitor->shouldProcess(localId, key);

	if (DPL_PROCESS_REJECT != fr) {
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

string DustProdLightEntity::getString(DPLEntity token) {
	return values[token].valStr;
}

void DustProdLightEntity::initMetaEntity(DPLEntity entity, DPLEntity primaryType, string name, DPLEntity parent) {
	localId = entity;
	this->primaryType = primaryType;

	types.insert(primaryType);
	string id = DustProdLightRuntime::getMetaEntityId(primaryType, name, parent);
	values[DPL_MBI_ATT_ENTITY_GLOBALID].set(DPL_TOKEN_VAL_STRING, &id);

	DustProdLightRuntime::pRuntime->dataGlobal[id] = entity;
}
