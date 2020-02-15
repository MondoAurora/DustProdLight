/*
 * dpl.h
 *
 * DustProdLight API (STL version)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_H_
#define DPL_H_

#include <string>
#include <set>

using namespace std;

enum DPLTokenType {
	DPL_TOKEN_INVALID,

	DPL_TOKEN_VAL_BOOL,
	DPL_TOKEN_VAL_INT,
	DPL_TOKEN_VAL_DOUBLE,
	DPL_TOKEN_VAL_STRING,

	DPL_TOKEN_REF_SINGLE,
	DPL_TOKEN_REF_SET,
	DPL_TOKEN_REF_ARR,
	DPL_TOKEN_REF_MAP
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

typedef int DPLType;
typedef int DPLToken;
typedef int DPLEntity;

enum DPLFilterResponse {
	DPL_FILTER_SKIP, DPL_FILTER_PROCESS, DPL_FILTER_VISIT
};

class DPLVisitor {
public:
	virtual ~DPLVisitor() {};

	virtual void visitStart(DPLEntity entity, void *pHint) {};
	virtual void visitEnd(DPLEntity entity, void *pHint) {};

	virtual DPLFilterResponse shouldProcess(DPLEntity entity, DPLToken token) { return DPL_FILTER_PROCESS; }

	virtual void processValBool(DPLEntity entity, DPLToken token, bool val, void *pHint) {};
	virtual void processValInt(DPLEntity entity, DPLToken token, int val, void *pHint) {};
	virtual void processValDouble(DPLEntity entity, DPLToken token, double val, void *pHint) {};
	virtual void processValString(DPLEntity entity, DPLToken token, string val, void *pHint) {};

	virtual void processRefBegin(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {};
	virtual void processRefEnd(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {};

	virtual void* processBeginEntity(DPLEntity entity, int key, void* pHint) { return pHint; };
	virtual void* processEndEntity(DPLEntity entity, int key, void* pHint) { return pHint; };
};

class DPL {
public:
// state management
	static void init();
	static void shutdown();

// meta initialization
	static DPLType getType(string typeName);
	static DPLToken getToken(DPLType type, string tokenName, DPLTokenType tokenType);

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

#endif /* DPL_H_ */
