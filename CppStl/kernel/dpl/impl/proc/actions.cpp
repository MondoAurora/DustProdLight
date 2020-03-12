#include <vector>

#include <iostream>
#include "proc.h"

using namespace std;


DPLProcessResult DPLUActionStreamReader::dplProcess(DPLProcessState *pState) {
	DPLUProcCtxJson* pCtx = (DPLUProcCtxJson*) pState->getContext(DPLJsonCtxJson);

	if ( !inStream.is_open() ) {
		inStream.open(pCtx->fName);
		pCtx->pos = 0;
	}

	inStream.get(pCtx->c);
	pCtx->charRead = inStream.good();

	if ( pCtx->charRead ) {
		++pCtx->pos;
	} else {
		inStream.close();
	}


	return DPL_PROCESS_SUCCESS;
}

//void DPLUActionStreamReader::dplRelease(DPLProcessState *pState) {
//	if ( inStream.is_open() ) {
//		inStream.close();
//	}
//}

DPLProcessResult DPLUActionDump::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_DIALOG);

	if (DPLData::getBool(ctx, 1, false)) {
		cout << DPLData::getString(ctx, 2, false);
		return DPL_PROCESS_SUCCESS;
	} else {
		return DPL_PROCESS_REJECT;
	}
}









void DustProdLightProcNode::init(int nodeId_, DustProdLightProcSession *pSession_) {
	nodeId = nodeId_;
	pSession = pSession_;

	inSep = false;
	pos = 0;
	count = 0;

//	if ( pProc ) {
//		pProc->dplInit(&pSession->state);
//	}
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

		bool hasSep = DPL_ENTITY_INVALID != pNodeDef->separator;

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
