/*
 * dpluproc.cpp
 *
 * Process implementations
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl_proc.h"

#include <vector>

#include <iostream>

using namespace std;

/****************************
 *
 * DustProdLightProcState
 *
 ****************************/

int DustProdLightProcState::getRelay() {
	int ret = relayId;
	relayId = DPL_PROCESS_NO_ACTION;
	return ret;
}

DustProdLightProcState::~DustProdLightProcState() {

}

//void DustProdLightProcState::setProcessed(bool p) {
//	processed = p;
//}

void* DustProdLightProcState::getContext(int ctxId) {
	return pSession->getContext(ctxId);
}

DPLProcessResult DustProdLightProcState::requestRelay(int relayId_) {
	relayId = relayId_;
	return DPL_PROCESS_RELAY;
}

//DPLProcessResult DustProdLightProcState::requestRelay(int relayId_, bool processed_) {
//	relayId = relayId_;
//	processed = processed_;
//	return DPL_PROCESS_RELAY;
//}

/****************************
 *
 * DustProdLightProcNodeDef
 *
 ****************************/

DustProdLightProcNodeDef::DustProdLightProcNodeDef(int nodeId, DPLProcessNodeTypes nodeType_, int separator_,
		vector<int> members_) :
		id(nodeId), nodeType(nodeType_), separator(separator_), rep(DPL_PROCESS_NO_ACTION), min(0), max(
		INT_MAX) {
	members = members_;
}

DustProdLightProcNodeDef::DustProdLightProcNodeDef(int nodeId, int rep_, int min_, int max_, int separator_) :
		id(nodeId), nodeType(DPLU_PROC_NODE_REPEAT), separator(separator_), rep(rep_), min(min_), max(max_) {
}

/****************************
 *
 * DustProdLightProcNode
 *
 ****************************/

DustProdLightProcNode::DustProdLightProcNode(DustProdLightProcNodeDef *pNodeDef_) :
		pProc(NULL), pNodeDef(pNodeDef_) {
}

DustProdLightProcNode::DustProdLightProcNode(DPLProcessAction *pProc_) :
		pProc(pProc_), pNodeDef(NULL) {
}

DustProdLightProcNode::~DustProdLightProcNode() {

}

void DustProdLightProcNode::init(int nodeId_, DustProdLightProcSession *pSession_) {
	nodeId = nodeId_;
	pSession = pSession_;

	inSep = false;
	pos = 0;
	count = 0;
}

DPLProcessResult DustProdLightProcNode::process(DPLProcessState *pState) {
	if (pProc) {
		return pProc->dplProcess(pState);
	} else {
		switch (pNodeDef->nodeType) {
		case DPLU_PROC_NODE_REPEAT:
			return pState->requestRelay(inSep ? pNodeDef->separator : pNodeDef->rep);
		case DPLU_PROC_NODE_SEQUENCE:
			return pState->requestRelay(inSep ? pNodeDef->separator : pNodeDef->members[pos]);
		case DPLU_PROC_NODE_SELECT:
			return pState->requestRelay(pNodeDef->members[pos]);
		case DPLU_PROC_NODE_NULL:
			return DPL_PROCESS_REJECT;
		}
	}

	return DPL_PROCESS_REJECT;
}

DPLProcessResult DustProdLightProcNode::childReturned(DPLProcessResult childResult, DPLProcessState *pState) {
	if (pProc) {
		return pProc->dplChildReturned(childResult, pState);
	} else {

		bool hasSep = DPL_PROCESS_NO_ACTION != pNodeDef->separator;

		switch (pNodeDef->nodeType) {
		case DPLU_PROC_NODE_REPEAT:
			if (DPL_PROCESS_REJECT == childResult) {
				return ((pNodeDef->min < count) || (!inSep && count && hasSep)) ? DPL_PROCESS_REJECT : DPL_PROCESS_SUCCESS;
			} else if (DPL_PROCESS_SUCCESS == childResult) {
				if (!inSep) {
					++count;
					if ((INT_MAX != pNodeDef->max) && (pNodeDef->max <= count)) {
						return DPL_PROCESS_REJECT;
					}
					inSep = hasSep;
				}
				return DPL_PROCESS_ACCEPT;
			}
			break;
		case DPLU_PROC_NODE_SEQUENCE:
			if (DPL_PROCESS_SUCCESS == childResult) {
				if (!inSep) {
					++pos;
					if (pNodeDef->members.size() == pos) {
						return DPL_PROCESS_SUCCESS;
					}
					inSep = hasSep;
				}
				return DPL_PROCESS_ACCEPT;
			}
			break;
		case DPLU_PROC_NODE_SELECT:
			if (DPL_PROCESS_SUCCESS == childResult) {
				return DPL_PROCESS_SUCCESS;
			} else if (DPL_PROCESS_REJECT == childResult) {
				++count;
				return (pNodeDef->members.size() == count) ? DPL_PROCESS_REJECT : DPL_PROCESS_ACCEPT;
			}
			break;
		case DPLU_PROC_NODE_NULL:
			return DPL_PROCESS_REJECT;
		}
	}

	return DPL_PROCESS_REJECT;
}

/****************************
 *
 * DustProdLightProcSession
 *
 ****************************/

DustProdLightProcSession::DustProdLightProcSession(DustProdLightProcEnv *pEnv_) :
		pEnv(pEnv_), state(this), result(DPL_PROCESS_ACCEPT) {
}

DustProdLightProcSession::~DustProdLightProcSession() {
}

void DustProdLightProcSession::open(const void* pInitData) {
	this->initData = pInitData;
	selectNode(pEnv->pDef->getStartNode());
	result = DPL_PROCESS_ACCEPT;
}

void* DustProdLightProcSession::getContext(int ctxId) {
	void *pCtx = mapCtx[ctxId];

	if (!pCtx) {
		pCtx = pEnv->logicFactory[ctxId]->createLogic(ctxId);
		mapCtx[ctxId] = pCtx;
		pEnv->pDef->openProcessContext(ctxId, pCtx, initData);
	}

	return pCtx;
}

void DustProdLightProcSession::finish(bool error) {
	while (!stack.empty()) {
		stepUp();
//		DustProdLightProcNode* pn = stack.top();
//		stack.pop();
//		pEnv->releaseProcessor(pn);
//
//		result = pn->childReturned(result, &state);
	}

	cout << endl << "stack released";

	for (map<int, void*>::iterator iCtx = mapCtx.begin(); mapCtx.end() != iCtx; ++iCtx) {
		pEnv->pDef->closeProcessContext(iCtx->first, iCtx->second);
	}

	initData = NULL;
}

DustProdLightProcNode* DustProdLightProcSession::selectNode(int nodeId) {
	DustProdLightProcNode* pn = pEnv->getProcessor(nodeId);
	stack.push(pn);

	pn->init(nodeId, this);

	result = DPL_PROCESS_ACCEPT;

	return pn;
}

void DustProdLightProcSession::stepUp() {
	DustProdLightProcNode* pn = stack.top();
	stack.pop();
	if (!stack.empty()) {
		result = stack.top()->childReturned(result, &state);
	}
	pEnv->releaseProcessor(pn);

}

//void DustProdLightProcSession::walkUp() {
//	while (DPL_PROCESS_SUCCESS == result) {
//	}
//
//	if (!state.processed && (DPL_PROCESS_ACCEPT == result)) {
//		step();
//	}
//}

//void DustProdLightProcSession::walkDown() {
//	while (DPL_PROCESS_RELAY == result) {
//		DustProdLightProcNode* pn = selectNode(state.getRelay());
//		if (!state.processed) {
//			state.processed = true;
////			step();
//			result = pn->process(&state);
//		} else {
//			result = DPL_PROCESS_ACCEPT;
//		}
//	}
//}

void DustProdLightProcSession::step() {
	DustProdLightProcNode* proc;

	switch (result) {
	case DPL_PROCESS_ACCEPT:
		proc = stack.top();
		result = proc->process(&state);
		break;
	case DPL_PROCESS_RELAY:
		selectNode(state.getRelay());
		break;
	case DPL_PROCESS_REJECT:
	case DPL_PROCESS_SUCCESS:
		stepUp();
		break;
	}
}

DPLProcessResult DustProdLightProcSession::execute(const void *pInitData) {
	for (open(pInitData); !stack.empty(); ) {
		step();
	}

	finish(false);

	return result;
}

/****************************
 *
 * DustProdLightProcEnv
 *
 ****************************/

map<DPLNarrative, DPLLogicProvider*> DustProdLightProcEnv::logicFactory;

map<DPLNarrative, DustProdLightProcEnv*> DustProdLightProcEnv::environments;

DustProdLightProcEnv::~DustProdLightProcEnv() {

}

DustProdLightProcEnv* DustProdLightProcEnv::getEnv(DPLNarrative narrative) {
	DustProdLightProcEnv *pEnv = environments[narrative];

	return pEnv;
}

void DustProdLightProcEnv::shutdown() {

}

DustProdLightProcNode* DustProdLightProcEnv::getProcessor(int procId) {
	set<DustProdLightProcNode*> ps = processorPool[procId];
	DustProdLightProcNode *proc;

	if (ps.empty()) {
		DustProdLightProcNodeDef *pPND = ctrlNodeDefs[procId];
		proc =
				pPND ?
						new DustProdLightProcNode(pPND) :
						new DustProdLightProcNode((DPLProcessAction*) logicFactory[procId]->createLogic(procId));
	} else {
		proc = *ps.begin();
		ps.erase(proc);
	}

	return proc;
}

void DustProdLightProcEnv::releaseProcessor(DustProdLightProcNode* proc) {
	processorPool[proc->nodeId].insert(proc);
}

DPLProcessResult DustProdLightProcEnv::executeProcess(const void *pInitData) {
	DustProdLightProcSession* pSession;

	if (sessionPool.empty()) {
		pSession = new DustProdLightProcSession(this);
	} else {
		pSession = *sessionPool.begin();
		sessionPool.erase(pSession);
	}

	DPLProcessResult res = pSession->execute(pInitData);

	sessionPool.insert(pSession);

	return res;
}

/****************************
 *
 * DPL API
 *
 ****************************/

void DPLProc::registerLogicProvider(DPLLogicProvider *pLogicFactory) {
	for (int i = pLogicFactory->getCount(); i-- > 0;) {
		DustProdLightProcEnv::logicFactory[pLogicFactory->getId(i)] = pLogicFactory;
	}
}

void DPLProc::registerNarrative(DPLNarrative narrative, DPLProcessDefinition &procDef) {
	DustProdLightProcEnv::environments[narrative] = new DustProdLightProcEnv(&procDef);

}

void DPLProc::registerCtrlRepeat(DPLNarrative narrative, int nodeId, int what, int minCount, int maxCount, int optSep) {
	DustProdLightProcEnv *pEnv = DustProdLightProcEnv::getEnv(narrative);
	pEnv->ctrlNodeDefs[nodeId] = new DustProdLightProcNodeDef(nodeId, what, minCount, maxCount, optSep);
}

void DPLProc::registerCtrlSequence(DPLNarrative narrative, int nodeId, int optSep, ...) {
	DustProdLightProcEnv *pEnv = DustProdLightProcEnv::getEnv(narrative);

	vector<int> mm;

	va_list args;
	va_start(args, optSep);
	int mId;
	while (DPL_PROCESS_NO_ACTION != (mId = va_arg(args, int))) {
		mm.push_back(mId);
	}
	va_end(args);

	pEnv->ctrlNodeDefs[nodeId] = new DustProdLightProcNodeDef(nodeId, DPLU_PROC_NODE_SEQUENCE, optSep, mm);
}

void DPLProc::registerCtrlSelection(DPLNarrative narrative, int nodeId, int members_...) {
	DustProdLightProcEnv *pEnv = DustProdLightProcEnv::getEnv(narrative);

	vector<int> mm;

	va_list args;
	va_start(args, members_);
	int mId;
	while (DPL_PROCESS_NO_ACTION != (mId = va_arg(args, int))) {
		mm.push_back(mId);
	}
	va_end(args);

	pEnv->ctrlNodeDefs[nodeId] = new DustProdLightProcNodeDef(nodeId, DPLU_PROC_NODE_SELECT, DPL_PROCESS_NO_ACTION, mm);
}

DPLProcessResult DPLProc::executeProcess(DPLNarrative narrative, const void *pInitData) {
	return DustProdLightProcEnv::getEnv(narrative)->executeProcess(pInitData);
}
