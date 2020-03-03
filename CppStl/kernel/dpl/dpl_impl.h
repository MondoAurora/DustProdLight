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

class DustProdLightValue {
private:
	DPLTokenType tokenType;

	int valInt;
	double valDbl;
	string valStr;

public:
	virtual ~DustProdLightValue();

	void set(DPLTokenType tokenType, const void* pVal);
	void optVisit(DPLVisitor *pVisitor, DPLEntity entity, DPLEntity token, void *pHint);

	friend DustProdLightStore;
	friend DPLMeta;
	friend DPLData;
};

class DustProdLightEntity {
	int localId = -1;
	DPLTokenType tokenType;

	DPLEntity store;
	DPLEntity primaryType;
	set<DPLEntity> types;
	bool changed;

	map<int, DustProdLightValue> values;
	map<int, DustProdLightRef*> refs;

	void updated();
	void optReloadMeta();

	friend DustProdLightStore;
	friend DustProdLightRef;
	friend DPLMeta;
	friend DPLData;


public:
	~DustProdLightEntity();

	bool isOfType(DPLEntity type);
	void getAllTypes(set<DPLEntity>& typeSet);

	void* optVisit(DPLVisitor *pVisitor, int key, void *pHint);
};

typedef vector<DustProdLightRef*>::iterator RefVectorIterator;

typedef map<int, DustProdLightValue>::iterator EntityValIterator;
typedef map<int, DustProdLightRef*>::iterator EntityRefIterator;

class DustProdLightRef {
	DPLEntity token;
	DPLTokenType tokenType;

	DPLEntity source;
	DPLEntity target;

	DPLEntity mapKey = -1;

	vector<DustProdLightRef*>* collection = 0;

	DustProdLightRef(DPLEntity ptoken, DPLTokenType ptokentype, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightEntity* ptoken, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightRef *porig, DPLEntity ptarget, int pkey);

	DustProdLightRef* getBy(DPLEntity ptarget, int key);
	void append(DustProdLightRef* pRef, int key);

	int getCount();
	DPLEntity getRef(int key);
	DPLEntity getTokenByIndex(int idx);

	void optVisit(DPLVisitor *pVisitor, void *pHint);
	void doVisit(DPLVisitor *pVisitor, int key, void *pHint, DPLFilterResponse fr);

public:
	~DustProdLightRef();

	friend DustProdLightStore;
	friend DustProdLightEntity;
	friend DPLMeta;
	friend DPLData;
};

class DustProdLightStore {
private:
	static DustProdLightStore* store;

	map<int, DustProdLightEntity> dataLocal;
	map<string, int> dataGlobal;
	set<DustProdLightRef*> refs;

	int nextEntityId;

	void createBootEntity(DPLEntity entity, const char* name, DPLEntity primaryType, int owner = 0, int hint = 0);

protected:
	string getMetaEntityId(DPLTokenType tokenType, string name, DPLEntity parent);
	DPLEntity getMetaEntity(DPLTokenType tokenType, string name, DPLEntity parent);

	DustProdLightEntity *validateGetToken(DPLEntity token, DPLTokenType tokenType);

	DustProdLightValue *getValue(DPLEntity entity, DPLTokenType tokenType, DPLEntity token);
	void setValue(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void* pVal);

	bool chgRef(DPLChange chg, DPLEntity entity, DPLEntity token, DPLEntity target, int key);
	DustProdLightRef* getRef(DPLEntity, DPLEntity);

	DustProdLightEntity* createEntity(DPLEntity primaryType);
	DustProdLightEntity* getEntity(DPLEntity entity);

public:
	DustProdLightStore();

	virtual ~DustProdLightStore();

	friend DustProdLightRef;
	friend DustProdLightEntity;
	friend DPLMeta;
	friend DPLData;
	friend DPLUtils;
};

#endif /* DPL_IMPL_H_ */
