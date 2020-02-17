/*
 * dpl_impl.h
 *
 * DustProdLight STL version - all implementation class declaration
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_H_
#define DPL_IMPL_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include "dpl.h"
#include "dplujson.h"

using namespace std;

class DustProdLightStore;
class DustProdLightRef;
class DustProdLightEntity;
class DustProdLightRef;

class DustProdLightToken {
private:
	int id = -1;
	DPLType type;
	DPLTokenType tokenType;
	string idStr;
	string name;

	friend DustProdLightStore;
	friend DustProdLightRef;
	friend DustProdLightEntity;
	friend DPL;
	friend DPLUtils;
};


class DustProdLightValue {
private:
	DPLTokenType tokenType;

	int valInt;
	double valDbl;
	string valStr;

public:
	virtual ~DustProdLightValue();

	void set(DPLTokenType tokenType, void* pVal);
	void optVisit(DPLVisitor *pVisitor, DPLEntity entity, DPLToken token, void *pHint);

	friend DustProdLightStore;
	friend DPL;
};

class DustProdLightEntity {
	int localId = -1;
	DPLType primaryType;
	set<DPLType> types;
	bool changed;

	map<int, DustProdLightValue> values;
	map<int, DustProdLightRef*> refs;

	void updated();
	void optReloadMeta();

	friend DustProdLightStore;
	friend DustProdLightRef;
	friend DPL;


public:
	~DustProdLightEntity();

	bool isOfType(DPLType type);
	void getAllTypes(set<DPLType>& typeSet);

	void* optVisit(DPLVisitor *pVisitor, int key, void *pHint);

};

typedef vector<DustProdLightRef*>::iterator RefVectorIterator;

typedef map<int, DustProdLightValue>::iterator EntityValIterator;
typedef map<int, DustProdLightRef*>::iterator EntityRefIterator;

class DustProdLightRef {
	DPLToken token;
	DPLTokenType tokenType;

	DPLEntity source;
	DPLEntity target;

	DPLToken mapKey = -1;

	vector<DustProdLightRef*>* collection = 0;

	DustProdLightRef(DPLToken ptoken, DPLTokenType ptokentype, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightToken* ptoken, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightRef *porig, DPLEntity ptarget, int pkey);

	DustProdLightRef* getBy(DPLEntity ptarget, int key);
	void append(DustProdLightRef* pRef, int key);

	int getCount();
	DPLEntity getRef(int key);
	DPLToken getTokenByIndex(int idx);

	void optVisit(DPLVisitor *pVisitor, void *pHint);
	void doVisit(DPLVisitor *pVisitor, int key, void *pHint, DPLFilterResponse fr);

public:
	~DustProdLightRef();

	friend DustProdLightStore;
	friend DustProdLightEntity;
	friend DPL;
};

class DustProdLightStore {
private:
	static DustProdLightStore* store;

	vector<string> vecTypes;
	map<string, int> mapTypes;

	vector<DustProdLightToken*> vecTokens;
	map<string, DustProdLightToken> mapTokens;

	map<int, DustProdLightEntity> dataLocal;
	map<string, DustProdLightEntity*> dataGlobal;
	set<DustProdLightRef*> refs;

	int nextEntityId = 0;

protected:
	DPLType getType(string typeName);
	DustProdLightToken *getToken(DPLType type, string tokenName, DPLTokenType tokenType);

	DustProdLightToken *validateGetToken(DPLToken token, DPLTokenType tokenType);

	DustProdLightValue *getValue(DPLEntity entity, DPLTokenType tokenType, DPLToken token);
	void setValue(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void* pVal);

	bool chgRef(DPLChange chg, DPLEntity entity, DPLToken token, DPLEntity target, int key);
	DustProdLightRef* getRef(DPLEntity, DPLToken);

	DustProdLightEntity* createEntity(DPLType primaryType);
	DustProdLightEntity* getEntity(DPLEntity entity);

public:
	DustProdLightStore();

	virtual ~DustProdLightStore();

	friend DustProdLightRef;
	friend DustProdLightEntity;
	friend DPL;
	friend DPLUtils;
};

#endif /* DPL_IMPL_H_ */
