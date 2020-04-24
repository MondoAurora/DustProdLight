
#include <vector>
#include <iostream>

#include "dpl_stl.h"

using namespace std;
using namespace DPLUnitText;
using namespace DPLUnitTools;
using namespace DPLUnitDialog;
using namespace DPLUnitNarrative;

DPLProcessResult DPLUActionStreamReader::dplActionExecute() {
	DPLEntity eDlg = DPLData::getEntityByPath(AgentDialog, DPL_ENTITY_INVALID);
	bool ok;

	if (!inStream.is_open()) {
		DPLEntity eSelf = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
		pos = 0;
		string sName = DPLData::getString(eSelf, AttStreamURL, "");
		inStream.open(sName);

		if ( !inStream.is_open() ) {
			return DPL_PROCESS_REJECT;
		}
	} else {
		ok = DPLData::getBool(eDlg, AttStreamOK, true);
		if (!ok) {
			inStream.close();
			return (1 < pos) ? DPL_PROCESS_ACCEPT : DPL_PROCESS_REJECT;
		}
	}

	char chr;
	inStream.get(chr);
	ok = inStream.good();

	DPLData::setBool(eDlg, AttStreamOK, ok);
	DPLData::setInt(eDlg, AttStreamPos, ++pos);
	DPLData::setInt(eDlg, AttCharacterChar, chr);

	return DPL_PROCESS_ACCEPT_PASS;
}

void DPLUActionStreamReader::dplRelease() {
//	if (inStream.is_open()) {
//		inStream.close();
//	}
}

DPLProcessResult DPLUActionDump::dplActionExecute() {
	DPLEntity eDialog = DPLData::getEntityByPath(AgentDialog, DPL_ENTITY_INVALID);

	if (DPLData::getBool(eDialog, AttStreamOK, false)) {
		char c = (char) DPLData::getInt(eDialog, AttCharacterChar, 0);
		cout << c;
		return DPL_PROCESS_ACCEPT_PASS;
	} else {
		string str = DPLData::getString(eDialog, AttTextString, "");

		if (!str.length()) {
			DPLEntity eSelf = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
			str = DPLData::getString(eSelf, AttTextString, "");
		}

		if (str.length()) {
			cout << str << endl;
			return DPL_PROCESS_ACCEPT;
		}

		return DPL_PROCESS_ACCEPT;
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
}

DPLProcessResult ProcActionControl::optGetChildResult(DPLProcessResult defRet ) {
	if (firstCall) {
		firstCall = false;
		return defRet;
	} else {
		return DustProdLightRuntime::getCurrentCore()->lastResult;
	}
}

ProcActionControl::~ProcActionControl() {
	for (BlockIterator iter = mapChildBlocks.begin(); iter != mapChildBlocks.end(); ++iter) {
		DustProdLightBlock *pb = iter->second;
		delete pb;
	}
}

DPLProcessResult ProcActionControl::optRelayChild() {
	DPLProcessResult ret = optGetChildResult();

	bool rp = DPL_PROCESS_ACCEPT_PASS == ret;

	if (rp || (DPL_PROCESS_ACCEPT == ret)) {
		DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);

		bool coll = isColl();
		unsigned int limit = coll ? DPLData::getRefCount(ctx, RefCollectionMembers) : (unsigned) DPLData::getInt(ctx, DPLUnitTools::AttLimitsIntMax, INT16_MAX);

		if (pos < limit) {
			DPLEntity r = coll ? DPLData::getRef(ctx, RefCollectionMembers, pos) : DPLData::getRef(ctx, RefLinkTarget, 0);
			DPLEntity eSep = DPLData::getRef(ctx, RefCollectionSeparator, 0);

			if (eSep) {
				if (inSep) {
					inSep = false;
					r = eSep;
				} else {
					inSep = true;
					++pos;
				}
			} else {
				++pos;
			}

			requestRelay(r);
			ret = DPL_PROCESS_READ;
		} else {
			ret = rp ? DPL_PROCESS_ACCEPT_PASS : DPL_PROCESS_ACCEPT;
		}
	}

	return ret;
}

DPLProcessResult ProcActionSequence::dplActionExecute() {
	return optRelayChild();
//	DPLProcessResult ret = optGetChildResult();
//
//	bool rp = DPL_PROCESS_ACCEPT_PASS == ret;
//
//	if (rp || (DPL_PROCESS_ACCEPT == ret)) {
//		DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
//
//		unsigned int count = DPLData::getRefCount(ctx, RefCollectionMembers);
//
//		if (pos < count) {
//			DPLEntity eSep = DPLData::getRef(ctx, RefCollectionSeparator, 0);
//			DPLEntity r = DPLData::getRef(ctx, RefCollectionMembers, pos);
//
//			if (eSep) {
//				if (inSep) {
//					inSep = false;
//					r = eSep;
//				} else {
//					inSep = true;
//					++pos;
//				}
//			} else {
//				++pos;
//			}
//
//			requestRelay(r);
//			ret = rp ? DPL_PROCESS_READ_REPEAT : DPL_PROCESS_READ_NEXT;
//		} else {
//			ret = rp ? DPL_PROCESS_ACCEPT_PASS : DPL_PROCESS_ACCEPT;
//		}
//	}
//
//	return ret;
}

DPLProcessResult ProcActionRepeat::dplActionExecute() {
	DPLProcessResult ret = optRelayChild();

	if ( DPL_PROCESS_REJECT == ret ) {
		DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
		int min = DPLData::getInt(ctx, DPLUnitTools::AttLimitsIntMin, 1);

		if ( min <= pos ) {
			ret = DPL_PROCESS_ACCEPT_PASS;
		}
	}

//	DPLProcessResult ret = optGetChildResult();
//	bool rp = DPL_PROCESS_ACCEPT_PASS == ret;
//
//	if (rp || (DPL_PROCESS_ACCEPT == ret)) {
//		DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
//		int count = DPLData::getInt(ctx, DPLUnitTools::AttLimitsIntMax, INT16_MAX);
//		DPLEntity eSep = DPLData::getRef(ctx, RefCollectionSeparator, 0);
//
//		if (pos < (unsigned) count) {
//			DPLEntity r = DPLData::getRef(ctx, RefLinkTarget, 0);
//
//			if (eSep) {
//				if (inSep) {
//					inSep = false;
//					r = eSep;
//				} else {
//					inSep = true;
//					++pos;
//				}
//			} else {
//				++pos;
//			}
//
//			requestRelay(r);
//			ret = rp ? DPL_PROCESS_READ_REPEAT : DPL_PROCESS_READ_NEXT;
//		} else {
//			ret = rp ? DPL_PROCESS_ACCEPT_PASS : DPL_PROCESS_ACCEPT;
//		}
//	}

	return ret;
}

DPLProcessResult ProcActionSelect::dplActionExecute() {
	DPLProcessResult ret = optGetChildResult(DPL_PROCESS_REJECT);

	if (DPL_PROCESS_REJECT == ret) {
		DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF, DPL_ENTITY_INVALID);
		unsigned int count = DPLData::getRefCount(ctx, RefCollectionMembers);

		if (pos < count) {
			DPLEntity r = DPLData::getRef(ctx, RefLinkTarget, pos++);
			requestRelay(r);
			ret = DPL_PROCESS_READ;
		}
	}

	return ret;
}

