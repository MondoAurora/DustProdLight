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

#include "../dpl.h"


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

	friend DPLData;
	friend class DustProdLightEntity;
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

	void initMetaEntity(DPLEntity entity, DPLTokenType tokenType, string name, DPLEntity parent);

	friend DustProdLightRef;
	friend DPLData;
	friend class DustProdLightImplementation;
	friend class DustProdLightRuntime;

public:
	~DustProdLightEntity();

	bool isOfType(DPLEntity type);
	void getAllTypes(set<DPLEntity>& typeSet);

	void* optVisit(DPLVisitor *pVisitor, int key, void *pHint);

	string getString(DPLEntity token);
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

	friend DustProdLightEntity;
	friend DPLData;
	friend class DustProdLightRuntime;
};

#endif /* DPL_IMPL_DATA_H_ */
