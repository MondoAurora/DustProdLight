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

typedef int DPLEntity;

enum DPLTokenType {
	DPL_ENTITY_INVALID,

	DPL_TOKEN_ENTITY,

	DPL_TOKEN_STORE,
	DPL_TOKEN_UNIT,

	DPL_TOKEN_NARRATIVE,
	DPL_TOKEN_ACTION,

	DPL_TOKEN_TYPE,

	DPL_TOKEN_VAL_BOOL,
	DPL_TOKEN_VAL_INT,
	DPL_TOKEN_VAL_DOUBLE,
	DPL_TOKEN_VAL_STRING,

	DPL_TOKEN_REF_SINGLE,
	DPL_TOKEN_REF_SET,
	DPL_TOKEN_REF_ARR,
	DPL_TOKEN_REF_MAP,

	DPL_TOKEN_
};

enum DPLContext {
	DPL_CTX_SELF = DPL_TOKEN_, DPL_CTX_CMD, DPL_CTX_MSG, DPL_CTX_APP, DPL_CTX_SYS, DPL_CTX_
};

enum DPLChange {
	DPL_CHG_REF_SET = DPL_CTX_, DPL_CHG_REF_REMOVE, DPL_CHG_REF_CLEAR, DPL_CHG_REF_
};

enum DPLProcessResult {
	DPL_PROCESS_REJECT = DPL_CHG_REF_, DPL_PROCESS_SUCCESS, DPL_PROCESS_ACCEPT, DPL_PROCESS_RELAY, DPL_PROCESS_
};

enum DPLFilterResponse {
	DPL_FILTER_SKIP = DPL_PROCESS_, DPL_FILTER_PROCESS, DPL_FILTER_VISIT, DPL_FILTER_
};

#define DPL_LAST_CONST DPL_FILTER_

#define REFKEY_ARR_APPEND -1

#define DPL_SEP_ID "."
#define DPL_SEP_STORE ":"


class DPLErrInvalidValueType {
};
class DPLErrUnkownField {
};
class DPLErrOutOfRange {
};
class DPLErrIdOutOfRange {
};

class DPLProcessState {
public:
	virtual ~DPLProcessState() {
	}
	virtual void* getContext(DPLEntity ctxId) = 0;
	virtual DPLProcessResult requestRelay(DPLEntity relayId_) = 0;
};

class DPLProcessAction {
public:
	virtual ~DPLProcessAction() {
	}

	virtual bool dplIsReusable() {
		return true;
	}
	virtual void dplInit(DPLProcessState *pState) {
	}
	virtual void dplRelease(DPLProcessState *pState) {
	}

	virtual DPLProcessResult dplProcess(DPLProcessState *pState) = 0;

	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse, DPLProcessState *pState) {
		return DPL_PROCESS_REJECT;
	}
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

		for (int idx = 0; DPL_ENTITY_INVALID != (mId = ids_[idx]); ++idx) {
			idMap[mId] = idx;
			providedIDs.push_back(mId);
		}
	}

	virtual ~DPLLogicProvider() {
	}

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

class DPLVisitor {
public:
	virtual ~DPLVisitor() {
	}

	virtual void visitStart(DPLEntity entity, void *pHint) {
	}
	virtual void visitEnd(DPLEntity entity, void *pHint) {
	}

	virtual DPLFilterResponse shouldProcess(DPLEntity entity, DPLEntity token) {
		return DPL_FILTER_PROCESS;
	}

	virtual void processValBool(DPLEntity entity, DPLEntity token, bool val, void *pHint) {
	}
	virtual void processValInt(DPLEntity entity, DPLEntity token, int val, void *pHint) {
	}
	virtual void processValDouble(DPLEntity entity, DPLEntity token, double val, void *pHint) {
	}
	virtual void processValString(DPLEntity entity, DPLEntity token, string val, void *pHint) {
	}

	virtual void processRefBegin(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void *pHint) {
	}
	virtual void processRefEnd(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void *pHint) {
	}

	virtual void* processBeginEntity(DPLEntity entity, int key, void* pHint) {
		return pHint;
	}
	virtual void* processEndEntity(DPLEntity entity, int key, void* pHint) {
		return pHint;
	}
};

class DPLMain {
public:
	static void init();
	static void registerLogicProvider(DPLLogicProvider *pLogicFactory);
	static DPLProcessResult send(DPLEntity target, DPLEntity command, DPLEntity param);
	static void shutdown();
};

//class DPLMeta {
//public:
//// meta initialization
//	static DPLEntity getUnit(string unitName);
//	static DPLEntity getType(DPLEntity unit, string typeName);
//	static DPLEntity getToken(DPLEntity type, string tokenName, DPLTokenType tokenType);
//
//	static DPLEntity getToken(string tokenId);
//};

class DPLData {
public:
// meta access
	static DPLEntity getMetaEntity(DPLTokenType tokenType, string name, DPLEntity parent = DPL_ENTITY_INVALID);
	static DPLEntity getEntityById(string globalId);

// meta detection on Entity
	static DPLEntity getPrimaryType(DPLEntity entity);
	static bool hasType(DPLEntity entity, DPLEntity type);
	static void getAllTypes(DPLEntity entity, set<DPLEntity>& typeSet);

// Entity creation and access
	static DPLEntity getEntityByPath(DPLContext ctx, ... );
	static DPLEntity createEntity(DPLEntity primaryType);
	static void visit(DPLEntity root, DPLVisitor *pVisitor, void *pHint);

// Entity value access
	static bool getBool(DPLEntity entity, DPLEntity token, bool defValue);
	static int getInt(DPLEntity entity, DPLEntity token, int defValue);
	static double getDouble(DPLEntity entity, DPLEntity token, double defValue);
	static string getString(DPLEntity entity, DPLEntity token, string defValue);

	static void setBool(DPLEntity entity, DPLEntity token, bool val);
	static void setInt(DPLEntity entity, DPLEntity token, int val);
	static void setDouble(DPLEntity entity, DPLEntity token, double val);
	static void setString(DPLEntity entity, DPLEntity token, string val);

// Entity reference access
	static int getRefCount(DPLEntity entity, DPLEntity token);
	static DPLEntity getRefKey(DPLEntity entity, DPLEntity token, int idx);
	static DPLEntity getRef(DPLEntity entity, DPLEntity token, int key);

	static bool setRef(DPLEntity entity, DPLEntity token, DPLEntity target, int key);
};

//class DPLProc {
//public:
//	static bool isRunning(DPLProcessResult result) {
//		return (DPL_PROCESS_ACCEPT == result) || (DPL_PROCESS_RELAY == result);
//	}
//	static void registerNarrative(DPLEntity narrative, DPLProcessDefinition &procDef);
//
//	static void registerCtrlRepeat(DPLEntity narrative, int nodeId, int what, int minCount, int maxCount, int optSep);
//	static void registerCtrlSequence(DPLEntity narrative, int nodeId, int optSep, ...);
//	static void registerCtrlSelection(DPLEntity narrative, int nodeId, int members_...);
//
//	static DPLProcessResult executeProcess(DPLEntity narrative, const void *initData);
//
//};

#endif /* DPL_H_ */
