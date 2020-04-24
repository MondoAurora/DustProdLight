/*
 * dpl_impl_data.h
 *
 * DustProdLight STL version - DPLData implementation declarations
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_DATA_H_
#define DPL_IMPL_DATA_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include <dpl.h>


class DustProdLightRef;
class DustProdLightEntity;
class DustProdLightRef;
class DustProdLightStore;

class DustProdLightValue {
private:
	DPLTokenType tokenType;

	int valInt = 0;
	double valDbl = 0.0;
	string valStr;

public:
	DustProdLightValue() : tokenType(DPL_ENTITY_INVALID), valInt(0), valDbl(0.0) {}
	DustProdLightValue(const DustProdLightValue &v) : tokenType(v.tokenType), valInt(v.valInt), valDbl(v.valDbl), valStr(v.valStr) {}

	virtual ~DustProdLightValue();

	void set(DPLTokenType tokenType, const void* pVal);
	void optVisit(DPLVisitor *pVisitor, DPLEntity entity, DPLEntity token, void *pHint);

	friend DPLData;
	friend class DustProdLightEntity;
};

class DustProdLightEntity {
	int localId = -1;
//	DPLTokenType tokenType = DPL_ENTITY_INVALID;

	DPLEntity store = DPL_ENTITY_INVALID;
	DPLEntity primaryType = DPL_ENTITY_INVALID;
	set<DPLEntity> types;
	bool changed = true;

	map<int, DustProdLightValue> values;
	map<int, DustProdLightRef*> refs;

	map<DPLEntity, DPLNarrativeLogic*> *pActionByAction = NULL;

	void updated();
	void optReloadMeta();

	void initMetaEntity(DPLEntity entity, DPLEntity primaryType, string name, DPLEntity parent);

	friend class DustProdLightRef;
	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
	friend class DustProdLightBlock;
	friend class DustProdLightAgent;
	friend class DustProdLightStore;

public:
	DustProdLightEntity();
	DustProdLightEntity(const DustProdLightEntity& e);

	~DustProdLightEntity();

	DustProdLightValue *getValue(DPLEntity token);
	void setValue(DPLEntity token, DPLTokenType tokenType, void* pVal);
	bool chgRef(DPLChange chg, DustProdLightEntity *pToken, DPLEntity target, int key);

	bool isOfType(DPLEntity type);
	void getAllTypes(set<DPLEntity>& typeSet);

	void* optVisit(DPLVisitor *pVisitor, int key, void *pHint);

	string getString(DPLEntity token);
	DPLEntity getRefEntity(DPLEntity token, int key);

	DPLNarrativeLogic *getLogicByCommand(DPLEntity cmd);
	void releaseActions();
};

typedef vector<DustProdLightRef*>::iterator RefVectorIterator;

typedef map<int, DustProdLightValue>::iterator EntityValIterator;
typedef map<int, DustProdLightRef*>::const_iterator EntityRefIterator;

typedef map<DPLEntity, DPLNarrativeLogic*>::iterator ActionIterator;

class DustProdLightRef {
	DPLEntity token;
	DPLTokenType tokenType;

	DPLEntity source;
	DPLEntity target;

	DPLEntity mapKey = -1;

	vector<DustProdLightRef*>* collection = 0;

	DustProdLightRef(const DustProdLightRef& r);

	DustProdLightRef(DPLEntity ptoken, DPLTokenType ptokentype, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightEntity* ptoken, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightRef *porig, DPLEntity ptarget, int pkey);

	DustProdLightRef* getBy(DPLEntity ptarget, int key);
	void append(DustProdLightRef* pRef, int key);

	int getCount();
	DPLEntity getRef(int key);
	DPLEntity getTokenByIndex(int idx);

	void optVisit(DPLVisitor *pVisitor, void *pHint);
	void doVisit(DPLVisitor *pVisitor, int key, void *pHint, DPLProcessResult fr);

public:
	~DustProdLightRef();

	friend DustProdLightEntity;
	friend DPLData;
	friend class DustProdLightRuntime;
	friend class DustProdLightBlock;
	friend class DustProdLightAgent;
};

class DustProdLightStore {
	int lastEntityId;
	map<int, DustProdLightEntity> emapLocal;

public:
	DustProdLightStore();

	DustProdLightEntity *resolve(DPLEntity e);

	friend DustProdLightEntity;
	friend DPLData;
	friend class DustProdLightRuntime;
	friend class DustProdLightBlock;
	friend class DustProdLightAgent;
};

#endif /* DPL_IMPL_DATA_H_ */
