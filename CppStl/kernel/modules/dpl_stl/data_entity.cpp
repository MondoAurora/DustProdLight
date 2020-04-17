/*
 * dpl_impl_data.h
 *
 *	Value and Entity implementation (not too complicated)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include "data.h"
#include "proc_runtime.h"
#include "_dplgen_boot.h"
#include <_dplgen_module_dpl_stl.h>

#include <dplutils.h>

DustProdLightEntity::DustProdLightEntity() {
}

DustProdLightEntity::DustProdLightEntity(const DustProdLightEntity& e) :
		localId(e.localId), tokenType(e.tokenType), store(e.store), primaryType(e.primaryType), types(e.types), changed(
				e.changed), values(e.values) {
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

DPLNarrativeLogic* DustProdLightEntity::getLogicByCommand(DPLEntity cmd) {
	DPLEntity eAction = getRefEntity(DPLUnitModel::RefEntityActions, cmd);
	DPLNarrativeLogic* pAction = NULL;

	if ( pActionByAction ) {
		pAction = (*pActionByAction)[eAction];
	} else {
		pActionByAction = new map<DPLEntity, DPLNarrativeLogic*>();
	}

	if ( !pAction ) {
		pAction = DustProdLightRuntime::createAction(eAction);
		(*pActionByAction)[eAction] = pAction;
	}

	return pAction;
}

void DustProdLightEntity::releaseActions() {
	if (pActionByAction) {
		for (ActionIterator ait = pActionByAction->begin(); ait != pActionByAction->end(); ++ait) {
			DPLEntity eAction = ait->first;
			DPLNarrativeLogic* pAction = ait->second;
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

string DustProdLightEntity::getString(DPLEntity token) {
	return values[token].valStr;
}

void DustProdLightEntity::initMetaEntity(DPLEntity entity, DPLTokenType tokenType_, string name, DPLEntity parent) {
	localId = entity;
	tokenType = tokenType_;

	switch (tokenType) {
	case DPL_ENTITY_INVALID:
		primaryType = DPL_ENTITY_INVALID;
		break;
	case DPL_TOKEN_STORE:
		primaryType = DPL_MBI_TYPE_MODEL_STORE;
		break;
	case DPL_TOKEN_UNIT:
		primaryType = DPL_MBI_TYPE_MODEL_UNIT;
		break;
	case DPL_TOKEN_TYPE:
		primaryType = DPL_MBI_TYPE_IDEA_TYPE;
		break;
	case DPL_TOKEN_VAL_BOOL:
	case DPL_TOKEN_VAL_DOUBLE:
	case DPL_TOKEN_VAL_INT:
	case DPL_TOKEN_VAL_STRING:
		primaryType = DPL_MBI_TYPE_IDEA_ATTRIBUTE;
		break;
	case DPL_TOKEN_REF_SINGLE:
	case DPL_TOKEN_REF_SET:
	case DPL_TOKEN_REF_ARR:
	case DPL_TOKEN_REF_MAP:
		primaryType = DPL_MBI_TYPE_IDEA_REFERENCE;
		break;
	case DPL_TOKEN_LOGIC:
		primaryType = DPL_MBI_TYPE_NARRATIVE_ACTION;
		break;
	case DPL_TOKEN_SERVICE:
		primaryType = DPL_MBI_TYPE_IDEA_SERVICE;
		break;
	case DPL_TOKEN_COMMAND:
		primaryType = DPL_MBI_TYPE_IDEA_COMMAND;
		break;
	case DPL_TOKEN_ENTITY:
		primaryType = DPL_ENTITY_INVALID;
		break;
	case DPL_TOKEN_:
		primaryType = DPL_ENTITY_INVALID;
		break;
	}

	types.insert(primaryType);
	string id = DustProdLightRuntime::getMetaEntityId(tokenType, name, parent);
	values[DPL_MBI_ATT_ENTITY_GLOBALID].set(DPL_TOKEN_VAL_STRING, &id);

	DustProdLightRuntime::pRuntime->dataGlobal[id] = entity;
}
