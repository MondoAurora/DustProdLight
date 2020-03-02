/*
 * dpl.h
 *
 * DustProdLight API (STL version)
 *
 * LOG
 *
 * 20200302
 *   Moved the stream into processor, to make context free of implementation-level pointers (ctx to move into entity) - GOOD.
 *   Created init and release in processor, call it when moving it onto stack - FAIL:
 *     readers generally appear inside repeat, init/release called all time.
 *     Actually, the stream state works OUTSIDE the processor, in the Session context.
 *     QUESTION if this mixed approach is better
 *     NEEDED a safe release on quick exit,
 *       but it would also need managing all connected processors in Session regardless off stck state. ugly.
 *       however, Processors MAY have local state anywway, this is necessary
 *       LogicProvider level management would not be modular.
 *     CLOSED: this is TBD.
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_H_
#define DPL_H_

#include <string>
#include <set>
#include <vector>
#include <map>

using namespace std;

typedef int DPLType;
typedef int DPLToken;
typedef int DPLEntity;

typedef int DPLNarrative;
typedef int DPLAction;

enum DPLTokenType {
	DPL_TOKEN_INVALID,
	DPL_TOKEN_VAL_BOOL,
	DPL_TOKEN_VAL_INT,
	DPL_TOKEN_VAL_DOUBLE,
	DPL_TOKEN_VAL_STRING,
	DPL_TOKEN_REF_SINGLE,
	DPL_TOKEN_REF_SET,
	DPL_TOKEN_REF_ARR,
	DPL_TOKEN_REF_MAP,

	DPL_TOKEN_ACTION,
};

enum DPLChange {
	DPL_CHG_REF_SET, DPL_CHG_REF_REMOVE, DPL_CHG_REF_CLEAR,
};

#define REFKEY_ARR_APPEND -1

class DPLErrInvalidValueType {
};
class DPLErrUnkownField {
};
class DPLErrOutOfRange {
};
class DPLErrIdOutOfRange {
};

class DPLProcessImplementation {
};

enum DPLProcessResult {
	DPL_PROCESS_REJECT, DPL_PROCESS_SUCCESS, DPL_PROCESS_ACCEPT, DPL_PROCESS_RELAY,
};

#define DPL_PROCESS_NO_ACTION 0
#define DPL_PROCESS_DEFAULT_CONTEXT 0

class DPLProcessState {
public:
	virtual ~DPLProcessState() {
	}
	virtual void* getContext(int ctxId) = 0;
	virtual DPLProcessResult requestRelay(int relayId_) = 0;
};

class DPLProcessAction {
public:
	virtual ~DPLProcessAction() {
	}
//	virtual void dplInit(DPLProcessState *pState) {
//	}
	virtual DPLProcessResult dplProcess(DPLProcessState *pState) = 0;
	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse, DPLProcessState *pState) {
		return DPL_PROCESS_REJECT;
	}
//	virtual void dplRelease(DPLProcessState *pState) {
//	}
};

class DPLProcessDefinition {
public:
	virtual ~DPLProcessDefinition() {
	}

	virtual int getStartNode() = 0;

	virtual void openProcessContext(int ctxId, void* pCtx, const void *pData) {
	}
	virtual void closeProcessContext(int ctxId, void* pCtx) {
	}
};

class DPLLogicProvider {
	vector<int> providedIDs;
	map<int, int> idMap;

public:
	DPLLogicProvider(int *ids_) {
		int mId;

		for (int idx = 0; DPL_PROCESS_NO_ACTION != (mId = ids_[idx]); ++idx) {
			idMap[mId] = idx;
			providedIDs.push_back(mId);
		}
	}
	;

	virtual ~DPLLogicProvider() {
	}
	;

	int getCount() {
		return providedIDs.size();
	}

	int getId(int idx) {
		return providedIDs[idx];
	}

	int indexOf(int logicId) {
		return idMap[logicId];
	}

	virtual void* createLogic(int logicId) = 0;
	virtual void releaseLogic(int logicId, void* pLogic) = 0;
};

enum DPLFilterResponse {
	DPL_FILTER_SKIP, DPL_FILTER_PROCESS, DPL_FILTER_VISIT
};

class DPLVisitor {
public:
	virtual ~DPLVisitor() {
	}

	virtual void visitStart(DPLEntity entity, void *pHint) {
	}
	virtual void visitEnd(DPLEntity entity, void *pHint) {
	}

	virtual DPLFilterResponse shouldProcess(DPLEntity entity, DPLToken token) {
		return DPL_FILTER_PROCESS;
	}

	virtual void processValBool(DPLEntity entity, DPLToken token, bool val, void *pHint) {
	}
	virtual void processValInt(DPLEntity entity, DPLToken token, int val, void *pHint) {
	}
	virtual void processValDouble(DPLEntity entity, DPLToken token, double val, void *pHint) {
	}
	virtual void processValString(DPLEntity entity, DPLToken token, string val, void *pHint) {
	}

	virtual void processRefBegin(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {
	}
	virtual void processRefEnd(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {
	}

	virtual void* processBeginEntity(DPLEntity entity, int key, void* pHint) {
		return pHint;
	}
	virtual void* processEndEntity(DPLEntity entity, int key, void* pHint) {
		return pHint;
	}
};

class DPLMeta {
public:
// state management
	static void init();
	static void shutdown();

// meta initialization
	static DPLType getType(string typeName);
	static DPLToken getToken(DPLType type, string tokenName, DPLTokenType tokenType);
	static DPLToken getToken(string tokenId);

};

class DPLData {
public:

// meta detection on Entity
	static DPLType getPrimaryType(DPLEntity entity);
	static bool hasType(DPLEntity entity, DPLType type);
	static void getAllTypes(DPLEntity entity, set<DPLType>& typeSet);

// Entity creation and access
	static DPLEntity getEntityByPath(DPLEntity root, int path... );
	static DPLEntity createEntity(DPLType primaryType);
	static void visit(DPLEntity root, DPLVisitor *pVisitor, void *pHint);

// Entity value access
	static bool getBool(DPLEntity entity, DPLToken token, bool defValue);
	static int getInt(DPLEntity entity, DPLToken token, int defValue);
	static double getDouble(DPLEntity entity, DPLToken token, double defValue);
	static string getString(DPLEntity entity, DPLToken token, string defValue);

	static void setBool(DPLEntity entity, DPLToken token, bool val);
	static void setInt(DPLEntity entity, DPLToken token, int val);
	static void setDouble(DPLEntity entity, DPLToken token, double val);
	static void setString(DPLEntity entity, DPLToken token, string val);

// Entity reference access
	static int getRefCount(DPLEntity entity, DPLToken token);
	static DPLToken getRefKey(DPLEntity entity, DPLToken token, int idx);
	static DPLEntity getRef(DPLEntity entity, DPLToken token, int key);

	static bool setRef(DPLEntity entity, DPLToken token, DPLEntity target, int key);
};

class DPLProc {
public:
	static bool isRunning(DPLProcessResult result) {
		return (DPL_PROCESS_ACCEPT == result) || (DPL_PROCESS_RELAY == result);
	}
	static void registerLogicProvider(DPLLogicProvider *pLogicFactory);

	static void registerNarrative(DPLNarrative narrative, DPLProcessDefinition &procDef);

	static void registerCtrlRepeat(DPLNarrative narrative, int nodeId, int what, int minCount, int maxCount, int optSep);
	static void registerCtrlSequence(DPLNarrative narrative, int nodeId, int optSep, ...);
	static void registerCtrlSelection(DPLNarrative narrative, int nodeId, int members_...);

	static DPLProcessResult executeProcess(DPLNarrative narrative, const void *initData);

};

#endif /* DPL_H_ */
