#include <vector>

#include <iostream>
#include "proc.h"

using namespace std;
using namespace DPLImplMeta;


DPLProcessResult DPLUActionStreamReader::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_DIALOG);
	bool ok;

	if ( !inStream.is_open() ) {
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
	if ( inStream.is_open() ) {
		inStream.close();
	}
}

DPLProcessResult DPLUActionDump::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_DIALOG);

	if (DPLData::getBool(ctx, AttStreamOK, false)) {
		cout << (char) DPLData::getInt(ctx, AttCharacterChar, 0);
		return DPL_PROCESS_SUCCESS;
	} else {
		return DPL_PROCESS_REJECT;
	}
}



DPLProcessResult ProcActionSignal::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_DIALOG);

	int pos = DPLData::getInt(ctx, AttDialogActiveAgent, 0);
	int count = DPLData::getRefCount(ctx, RefCollectionMembers);

	DPLData::setInt(ctx, AttDialogActiveAgent, (++pos < count) ? pos : 0);

	return DPL_PROCESS_SUCCESS;
}

DPLProcessResult requestRelay(DPLEntity relay) {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_AGENT);
	DPLData::setRef(ctx, RefAgentRelay, relay, 0);
	return DPL_PROCESS_RELAY;
}


DPLProcessResult ProcActionSequence::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	return requestRelay(inSep ? DPLData::getRef(ctx, RefCollectionSeparator, 0) : DPLData::getRef(ctx, RefCollectionMembers, pos));
}

DPLProcessResult ProcActionSequence::dplChildReturned(DPLProcessResult childResult) {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	bool hasSep = DPL_ENTITY_INVALID != DPLData::getRef(ctx, RefCollectionSeparator, 0);

	if (DPL_PROCESS_SUCCESS == childResult) {
		if (!inSep) {
			++pos;
			if (DPLData::getRefCount(ctx, RefCollectionMembers) == pos) {
				return DPL_PROCESS_SUCCESS;
			}
			inSep = hasSep;
		}
		return DPL_PROCESS_ACCEPT;
	}

	return DPL_PROCESS_REJECT;
}

DPLProcessResult ProcActionSelect::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	return requestRelay(DPLData::getRef(ctx, RefCollectionMembers, pos));
}

DPLProcessResult ProcActionSelect::dplChildReturned(DPLProcessResult childResult) {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);

	if (DPL_PROCESS_SUCCESS == childResult) {
		return DPL_PROCESS_SUCCESS;
	} else if (DPL_PROCESS_REJECT == childResult) {
		++pos;
		return (DPLData::getRefCount(ctx, RefCollectionMembers) == pos) ? DPL_PROCESS_REJECT : DPL_PROCESS_ACCEPT;
	}

	return DPL_PROCESS_REJECT;
}

DPLProcessResult ProcActionRepeat::dplProcess() {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	return requestRelay(inSep ? DPLData::getRef(ctx, RefCollectionSeparator, 0) : DPLData::getRef(ctx, RefLinkTarget, 0));
}

DPLProcessResult ProcActionRepeat::dplChildReturned(DPLProcessResult childResult) {
	DPLEntity ctx = DPLData::getEntityByPath(DPL_CTX_SELF);
	bool hasSep = DPL_ENTITY_INVALID != DPLData::getRef(ctx, RefCollectionSeparator, 0);

	if (DPL_PROCESS_REJECT == childResult) {
		return ((min < count) || (!inSep && count && hasSep)) ? DPL_PROCESS_REJECT : DPL_PROCESS_SUCCESS;
	} else if (DPL_PROCESS_SUCCESS == childResult) {
		if (!inSep) {
			++count;
			if ((INT_MAX != max) && (max <= count)) {
				return DPL_PROCESS_REJECT;
			}
			inSep = hasSep;
		}
		return DPL_PROCESS_ACCEPT;
	}

	return DPL_PROCESS_REJECT;
}

