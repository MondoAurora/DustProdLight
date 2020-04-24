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

using namespace std;

typedef int DPLEntity;

#define DPL_REFKEY_ARR_APPEND -1
#define DPL_ENTITY_APPEND -1
#define DPL_ERR_UNSPECIFIED -1

#define DPL_CTX_SELF -2


#define DPL_SEP_ID "."
#define DPL_SEP_STORE ":"


enum DPLTokenType {
	DPL_ENTITY_INVALID = 0,
	DPL_ENTITY_TRUE,
	DPL_ENTITY_FALSE,

	DPL_TOKEN_UNIT,
	DPL_TOKEN_TYPE,
	DPL_TOKEN_AGENT,
	DPL_TOKEN_CONST,
	DPL_TOKEN_TAG,

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

enum DPLProcessResult {
	DPL_PROCESS_NOTIMPLEMENTED = DPL_ENTITY_INVALID,
	DPL_PROCESS_REJECT = DPL_TOKEN_,
	DPL_PROCESS_ACCEPT_PASS,
	DPL_PROCESS_ACCEPT,
	DPL_PROCESS_ACCEPT_READ,
	DPL_PROCESS_READ,
	DPL_PROCESS_
};

enum DPLChange {
	DPL_CHG_REF_SET = DPL_PROCESS_, DPL_CHG_REF_REMOVE, DPL_CHG_REF_CLEAR, DPL_CHG_
};

#define DPL_LAST_CONST DPL_CHG_


class DPLError {
public:
	const DPLEntity eEvent;

	DPLError(DPLEntity eEvent_) : eEvent(eEvent_) {};
};


class DPLNarrativeResource {
public:
	virtual ~DPLNarrativeResource() {
	}

	virtual DPLProcessResult dplResourceInit() {
		return DPL_PROCESS_ACCEPT;
	}
	virtual DPLProcessResult dplResourceRelease() {
		return DPL_PROCESS_ACCEPT;
	}
};

class DPLNarrativeAction {
public:
	virtual ~DPLNarrativeAction() {
	}
	virtual DPLProcessResult dplActionExecute() {
		return DPL_PROCESS_NOTIMPLEMENTED;
	}
};


class DPLNarrativeLogic : public DPLNarrativeResource, public DPLNarrativeAction {
public:
	virtual ~DPLNarrativeLogic() {
	}
};

class DPLModule {

public:
	virtual ~DPLModule() {
	}

	virtual void init(DPLEntity eModule) {}
	virtual void release() {}

	virtual DPLNarrativeLogic* createLogic(int logicId) const {
		return NULL;
	}
	virtual DPLProcessResult dispatchCommand(int logicId, DPLNarrativeLogic* pLogic, DPLEntity cmd, DPLEntity param = DPL_ENTITY_INVALID) const {
		return DPL_PROCESS_NOTIMPLEMENTED;
	}
	virtual void releaseLogic(int logicId, DPLNarrativeLogic* pLogic) const {
	}
};

class DPLVisitor {
public:
	virtual ~DPLVisitor() {
	}

	virtual void visitStart(DPLEntity entity, void *pHint) {
	}
	virtual void visitEnd(DPLEntity entity, void *pHint) {
	}

	virtual DPLProcessResult shouldProcess(DPLEntity entity, DPLEntity token) {
		return DPL_PROCESS_ACCEPT_READ;
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
//	static void createBootEntities();
	static void registerModule(const char* moduleName, DPLModule *pModule);

	static DPLProcessResult run();

	static void shutdown();
};

class DPLData {
public:
// meta access
	static DPLEntity getMetaEntity(DPLEntity primaryType, string name, DPLEntity parent = DPL_ENTITY_INVALID);
	static DPLEntity getEntityById(string globalId);
	static DPLEntity getConst(DPLEntity primaryType, string name, DPLEntity unit);


// meta detection on Entity
	static DPLEntity getPrimaryType(DPLEntity entity);
	static bool hasType(DPLEntity entity, DPLEntity type);
	static void getAllTypes(DPLEntity entity, set<DPLEntity>& typeSet);

// Entity creation and access
	static DPLEntity getEntityByPath(DPLEntity ctx, ... );
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
	static unsigned int getRefCount(DPLEntity entity, DPLEntity token);
	static DPLEntity getRefKey(DPLEntity entity, DPLEntity token, int idx);
	static DPLEntity getRef(DPLEntity entity, DPLEntity token, int key = DPL_REFKEY_ARR_APPEND);

	static bool setRef(DPLEntity entity, DPLEntity token, DPLEntity target, int key = DPL_REFKEY_ARR_APPEND);
};


#endif /* DPL_H_ */
