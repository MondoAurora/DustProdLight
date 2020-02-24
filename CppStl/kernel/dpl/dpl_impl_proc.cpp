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

void DustProdLightProcState::setProcessed(bool p) {
	processed = p;
}

void* DustProdLightProcState::getContext(int ctxId) {
	return pSession->getContext(ctxId);
}

DPLProcessResult DustProdLightProcState::requestRelay(int relayId_, bool processed_) {
	relayId = relayId_;
	processed = processed_;
	return DPL_PROCESS_RELAY;
}



/****************************
 *
 * DustProdLightProcNodeDef
 *
 ****************************/

DustProdLightProcNodeDef::DustProdLightProcNodeDef(int nodeId, DPLProcessNodeTypes nodeType_, int separator_,
		vector<int> members_) :
		id(nodeId), nodeType(nodeType_), members(members_), separator(separator_), rep(DPL_PROCESS_NO_ACTION), min(0), max(
		INT_MAX) {
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
	}

	DustProdLightProcState *pS = (DustProdLightProcState *) pState;

	switch (pNodeDef->nodeType) {
	case DPLU_PROC_NODE_REPEAT:
		return pState->requestRelay(inSep ? pNodeDef->separator : pNodeDef->rep, false);
	case DPLU_PROC_NODE_SEQUENCE:
		return pState->requestRelay(inSep ? pNodeDef->separator : pNodeDef->members[pos], pS->processed);
	case DPLU_PROC_NODE_SELECT:
		return pState->requestRelay(pNodeDef->members[pos], pS->processed);
	case DPLU_PROC_NODE_NULL:
		return DPL_PROCESS_REJECT;
	}

	return DPL_PROCESS_REJECT;
}

DPLProcessResult DustProdLightProcNode::childReturned(DPLProcessResult childResult, DPLProcessState *pState) {
	bool hasSep = DPL_PROCESS_NO_ACTION == pNodeDef->separator;

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
				++count;
				if (pNodeDef->members.size() == count) {
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

	return DPL_PROCESS_REJECT;
}





/****************************
 *
 * DustProdLightProcSession
 *
 ****************************/

DustProdLightProcSession::DustProdLightProcSession(DustProdLightProcEnv *pEnv_) :
		pEnv(pEnv_), state(this), result(DPL_PROCESS_ACCEPT) {
//	procCtx = pEnv->pDef->createProcessContext();
}

DustProdLightProcSession::~DustProdLightProcSession() {
}

void DustProdLightProcSession::open(void* pInitData) {
//	pEnv->pDef->openProcessContext(procCtx, pInitData);
	selectNode(pEnv->pDef->getStartNode());
}

void* DustProdLightProcSession::getContext(int ctxId) {
	void *pCtx = mapCtx[ctxId];

	if ( !pCtx ) {
		pCtx = pEnv->pDef->createProcessContext(ctxId);
		mapCtx[ctxId] = pCtx;
	}

	return pCtx;
}

void DustProdLightProcSession::finish(bool error) {
	while (!stack.empty()) {
		DustProdLightProcNode* pn = stack.top();
		stack.pop();
		pEnv->releaseProcessor(pn);

		result = pn->childReturned(result, &state);
	}

	for (map<int, void*>::iterator iCtx = mapCtx.begin(); mapCtx.end() != iCtx; ++iCtx) {
		pEnv->pDef->closeProcessContext(iCtx->first, iCtx->second);
	}
}

DustProdLightProcNode* DustProdLightProcSession::selectNode(int nodeId) {
	DustProdLightProcNode* pn = pEnv->getProcessor(nodeId);
	stack.push(pn);

	pn->init(nodeId, this);

	return pn;
}

void DustProdLightProcSession::walkUp() {
	while (DPL_PROCESS_SUCCESS == result) {
		DustProdLightProcNode* pn = stack.top();
		stack.pop();
		pEnv->releaseProcessor(pn);

		result = pn->childReturned(result, &state);
	}

	if (!state.processed && (DPL_PROCESS_ACCEPT == result)) {
		step();
	}
}

void DustProdLightProcSession::walkDown() {
	while (DPL_PROCESS_RELAY == result) {
//		DustProdLightProcNode* pn = selectNode(state.getRelay());
		if (!state.processed) {
			state.processed = true;
			step();
//			result = pn->process(procCtx, &state);
		} else {
			result = DPL_PROCESS_ACCEPT;
		}
	}
}

DPLProcessResult DustProdLightProcSession::step() {
	DustProdLightProcNode* proc = stack.top();

	result = proc->process(&state);

	switch (result) {
	case DPL_PROCESS_ACCEPT:
		// fine, do nothing
		break;
	case DPL_PROCESS_REJECT:
		// do nothing here
		break;
	case DPL_PROCESS_RELAY:
		walkDown();
		break;
	case DPL_PROCESS_SUCCESS:
		walkUp();
		break;
	}

	if (DPL_PROCESS_REJECT == result) {
		finish(true);
	}

	return result;
}




/****************************
 *
 * DustProdLightProcEnv
 *
 ****************************/

map<DPLProcessDefinition*, DustProdLightProcEnv*> DustProdLightProcEnv::environments;

DustProdLightProcEnv::~DustProdLightProcEnv() {

}

DustProdLightProcEnv* DustProdLightProcEnv::getEnv(DPLProcessDefinition *pProcDef) {
	DustProdLightProcEnv *pEnv = environments[pProcDef];

	if (!pEnv) {
		pEnv = new DustProdLightProcEnv();
		pEnv->pDef = pProcDef;
	}

	return pEnv;
}

void DustProdLightProcEnv::shutdown() {

}

DustProdLightProcSession* DustProdLightProcEnv::getSession(void *pInitData) {
	DustProdLightProcSession* pSession;

	if (sessionPool.empty()) {
		pSession = new DustProdLightProcSession(this);
	} else {
		pSession = *sessionPool.begin();
		sessionPool.erase(pSession);
	}
	pSession->open(pInitData);

	return pSession;
}

void DustProdLightProcEnv::releaseSession(DustProdLightProcSession *pSession) {
	pSession->finish(false);
	sessionPool.insert(pSession);
}

DustProdLightProcNode* DustProdLightProcEnv::getProcessor(int procId) {
	set<DustProdLightProcNode*> ps = processorPool[procId];
	DustProdLightProcNode *proc;

	if (ps.empty()) {
		DustProdLightProcNodeDef *pPND = ctrlNodeDefs[procId];
		proc = pPND ? new DustProdLightProcNode(pPND) : new DustProdLightProcNode(pDef->createProcessor(procId));
	} else {
		proc = *ps.begin();
		ps.erase(proc);
	}

	return proc;
}

void DustProdLightProcEnv::releaseProcessor(DustProdLightProcNode* proc) {
	processorPool[proc->nodeId].insert(proc);
}

DPLProcessResult DustProdLightProcEnv::executeProcess(void *pInitData) {
	DPLProcessResult res = DPL_PROCESS_REJECT;

	DustProdLightProcSession* pSession = getSession(pInitData);

	do {
		res = pSession->step();
	} while (DPL_PROCESS_ACCEPT == res);

	releaseSession(pSession);

	return res;
}




/****************************
 *
 * DPL API
 *
 ****************************/

void DPLProc::registerCtrlRepeat(DPLProcessDefinition &procDef, int nodeId, int what, int minCount, int maxCount,
		int optSep) {
	DustProdLightProcEnv *pEnv = DustProdLightProcEnv::getEnv(&procDef);
	pEnv->ctrlNodeDefs[nodeId] = new DustProdLightProcNodeDef(nodeId, what, minCount, maxCount, optSep);
}

void DPLProc::registerCtrlSequence(DPLProcessDefinition &procDef, int nodeId, int optSep, int members_...) {
	DustProdLightProcEnv *pEnv = DustProdLightProcEnv::getEnv(&procDef);

	vector<int> mm;

	va_list args;
	va_start(args, members_);
	int mId;
	while (DPL_PROCESS_NO_ACTION != (mId = va_arg(args, int))) {
		mm.push_back(mId);
	}
	va_end(args);

	pEnv->ctrlNodeDefs[nodeId] = new DustProdLightProcNodeDef(nodeId, DPLU_PROC_NODE_SEQUENCE, optSep, mm);
}

void DPLProc::registerCtrlSelection(DPLProcessDefinition &procDef, int nodeId, int members_...) {
	DustProdLightProcEnv *pEnv = DustProdLightProcEnv::getEnv(&procDef);

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

DPLProcessResult DPLProc::executeProcess(DPLProcessDefinition &procDef, void *pInitData) {
	return DustProdLightProcEnv::getEnv(&procDef)->executeProcess(pInitData);
}
