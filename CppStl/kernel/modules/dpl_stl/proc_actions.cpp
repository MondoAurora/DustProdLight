#include "proc_actions.h"

#include <vector>

#include <iostream>

#include <_dplgen_module_dpl_stl.h>

using namespace std;
using namespace DPLUnitText;
using namespace DPLUnitTools;
using namespace DPLUnitDialog;

DPLProcessResult DPLUActionStreamReader::dplActionExecute() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_TRANSACTION);
	bool ok;

	if (!inStream.is_open()) {
		pos = 0;
		string sName = DPLData::getString(ctx, AttStreamURL, "");
		inStream.open(sName);
	} else {
		ok = DPLData::getBool(ctx, AttStreamOK, true);
		if (!ok) {
			inStream.close();
			return (1 < pos) ? DPL_PROCESS_SUCCESS : DPL_PROCESS_REJECT;
		}
	}

	char chr;
	inStream.get(chr);
	ok = inStream.good();

	DPLData::setBool(ctx, AttStreamOK, ok);
	DPLData::setInt(ctx, AttStreamPos, ++pos);
	DPLData::setInt(ctx, AttCharacterChar, chr);

	return DPL_PROCESS_SUCCESS;
}

void DPLUActionStreamReader::dplRelease() {
	if (inStream.is_open()) {
		inStream.close();
	}
}

DPLProcessResult DPLUActionDump::dplActionExecute() {
	DPLEntity eParam = DPLData::getEntityByPath(DPL_CTX_PARAM, DPL_ENTITY_INVALID);

	if (DPLData::getBool(eParam, AttStreamOK, false)) {
		cout << (char) DPLData::getInt(eParam, AttCharacterChar, 0);
		return DPL_PROCESS_SUCCESS;
	} else {
		string str = DPLData::getString(eParam, AttTextString, "");

		if (!str.length()) {
			DPLEntity eSelf = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
			str = DPLData::getString(eSelf, AttTextString, "");
		}

		if (str.length()) {
			cout << str << endl;
			return DPL_PROCESS_SUCCESS;
		}
		return DPL_PROCESS_REJECT;
	}
}

void ProcActionControl::requestRelay(DPLEntity relay) {
	DustProdLightAgent *pAgent = DustProdLightRuntime::getCurrentCore()->getDialog()->getAgent();
	DustProdLightBlock *pBlockCurrent = pAgent->getBlock();

	DustProdLightBlock *pBlockRelay = mapChildBlocks[relay];
	if (!pBlockRelay) {
		pBlockRelay = new DustProdLightBlock();
		DustProdLightEntity *pe = pBlockCurrent->getEntity(relay);
		pBlockRelay->init(pe, pBlockCurrent);
		mapChildBlocks[relay] = pBlockRelay;
	}

	pAgent->relayEntry(pBlockRelay);

//	if (!pa) {
//		pC->pAgent = pC->pBlock->pOwnAgent = pa = new DustProdLightAgent();
//		pa->stack[0] = pC->pBlock;
//	}
//
//	pa->stack[++pa->stackPos] = pb;
//	pC->pBlock = pb;
}

ProcActionControl::~ProcActionControl() {
	for (BlockIterator iter = mapChildBlocks.begin(); iter != mapChildBlocks.end(); ++iter) {
		DustProdLightBlock *pb = iter->second;
		delete pb;
	}
}

DPLProcessResult ProcActionSequence::dplActionExecute() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	requestRelay(
			inSep ? DPLData::getRef(ctx, RefCollectionSeparator, 0) : DPLData::getRef(ctx, RefCollectionMembers, pos));
	return DPL_PROCESS_ACCEPT;
}

//DPLProcessResult ProcActionSequence::dplChildReturned(DPLProcessResult childResult) {
//	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
//	bool hasSep = DPL_ENTITY_INVALID != DPLData::getRef(ctx, RefCollectionSeparator, 0);
//
//	if (DPL_PROCESS_SUCCESS == childResult) {
//		if (!inSep) {
//			++pos;
//			if (DPLData::getRefCount(ctx, RefCollectionMembers) == pos) {
//				return DPL_PROCESS_SUCCESS;
//			}
//			inSep = hasSep;
//		}
//		return DPL_PROCESS_ACCEPT;
//	}
//
//	return DPL_PROCESS_REJECT;
//}

DPLProcessResult ProcActionSelect::dplActionExecute() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	requestRelay(DPLData::getRef(ctx, RefCollectionMembers, pos));
	return DPL_PROCESS_ACCEPT;
}

//DPLProcessResult ProcActionSelect::dplChildReturned(DPLProcessResult childResult) {
//	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
//
//	if (DPL_PROCESS_SUCCESS == childResult) {
//		return DPL_PROCESS_SUCCESS;
//	} else if (DPL_PROCESS_REJECT == childResult) {
//		++pos;
//		return (DPLData::getRefCount(ctx, RefCollectionMembers) == pos) ? DPL_PROCESS_REJECT : DPL_PROCESS_ACCEPT;
//	}
//
//	return DPL_PROCESS_REJECT;
//}

DPLProcessResult ProcActionRepeat::dplActionExecute() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
	int max = DPLData::getInt(ctx, DPLUnitTools::AttLimitsIntMax, INT16_MAX);
	DPLEntity eSep = DPLData::getRef(ctx, RefCollectionSeparator, 0);

	if (count < (unsigned) max) {
		DPLEntity r = DPLData::getRef(ctx, RefLinkTarget, 0);

		if ( eSep ) {
			if ( inSep ) {
				inSep = false;
				r = eSep;
			} else {
				inSep = true;
				++count;
			}
		} else {
			++count;
		}

		requestRelay(r);
		return DPL_PROCESS_ACCEPT;
	} else {
		return DPL_PROCESS_SUCCESS;
	}
}

//DPLProcessResult ProcActionRepeat::dplChildReturned(DPLProcessResult childResult) {
//	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
//	bool hasSep = DPL_ENTITY_INVALID != DPLData::getRef(ctx, RefCollectionSeparator, 0);
//
//	if (DPL_PROCESS_REJECT == childResult) {
//		return ((min < count) || (!inSep && count && hasSep)) ? DPL_PROCESS_REJECT : DPL_PROCESS_SUCCESS;
//	} else if (DPL_PROCESS_SUCCESS == childResult) {
//		if (!inSep) {
//			++count;
//			if ((INT_MAX != max) && (max <= count)) {
//				return DPL_PROCESS_REJECT;
//			}
//			inSep = hasSep;
//		}
//		return DPL_PROCESS_ACCEPT;
//	}
//
//	return DPL_PROCESS_REJECT;
//}
