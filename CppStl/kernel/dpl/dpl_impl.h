/*
 * fleetmandata.h
 *
 *  Created on: Feb 12, 2020
 *      Author: loran
 */

#ifndef DPL_IMPL_H_
#define DPL_IMPL_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include "dpl.h"

using namespace std;

class DustProdLightStore;
class FleetManJSON;
class DustProdLightRef;
class DustProdLightEntity;
class DustProdLightRef;

class DustProdLightToken {
private:
	int id = -1;
	DPLType type;
	DPLValType valType;

	friend DustProdLightStore;
	friend FleetManJSON;
	friend DustProdLightRef;
	friend DustProdLightEntity;
};


class DustProdLightValue: public DPLValue {
private:
	int int_value;
	double double_value;
	string string_value;

public:
	virtual ~DustProdLightValue();

	virtual int getInt();
	virtual double getDouble();
	virtual string getString();

	void set(DPLValType valType, void* pVal);

	friend DustProdLightStore;
};

class DustProdLightEntity : public DPLEntityInt {
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

public:
	virtual ~DustProdLightEntity();

	virtual DPLType getPrimaryType();

	virtual bool isOfType(DPLType type);
	virtual void getTypes(set<DPLType>& typeSet);
};

typedef vector<DustProdLightRef*>::iterator RefVectorIterator;

class DustProdLightRef: public DPLRef {
	DPLToken token;
	DPLValType refType;

	DPLEntity source;
	DPLEntity target;

	DPLToken mapKey = -1;

	vector<DustProdLightRef*>* collection = 0;

	DustProdLightRef(DPLToken ptoken, DPLValType preftype, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightToken* ptoken, DPLEntity psource, DPLEntity ptarget, int pkey);
	DustProdLightRef(DustProdLightRef *porig, DPLEntity ptarget, int pkey);

	DustProdLightRef* getBy(DPLEntity ptarget, int key);
	void append(DustProdLightRef* pRef, int key);

public:
	~DustProdLightRef();

	virtual DPLValType getType();
	virtual int getCount();
	virtual DPLEntity getRef(int key);
	virtual DPLToken getTokenByIndex(int idx);

	friend DustProdLightStore;
};

class DustProdLightStore: public DPLStore {
private:
	static DustProdLightStore store;

	vector<string> vecTypes;
	map<string, int> mapTypes;

	vector<DustProdLightToken*> vecTokens;
	map<string, DustProdLightToken> mapTokens;

	map<int, DustProdLightEntity> dataLocal;
	map<string, DustProdLightEntity*> dataGlobal;
	set<DustProdLightRef*> refs;

	int nextEntityId = 0;

protected:
	DustProdLightToken *validateGetToken(DPLToken token, DPLValType valType);

	DustProdLightToken *getTokenImpl(DPLType type, string tokenName, DPLValType valType);

	virtual DPLType getTypeInt(string typeName);
	virtual DPLToken getTokenInt(DPLType type, string tokenName, DPLValType valType);

	virtual DPLValue *getValue(DPLEntity entity, DPLValType valType, DPLToken token);
	virtual void setValue(DPLEntity entity, DPLToken token, DPLValType valType, void* pVal);

	virtual bool chgRef(DPLChange chg, DPLEntity entity, DPLToken token, DPLEntity target, int key);
	virtual DPLRef* getRef(DPLEntity, DPLToken);

	virtual DPLEntity createEntityInt(DPLType primaryType);
	virtual DPLEntityInt* getEntityInt(DPLEntity entity);

public:
	DustProdLightStore();

	virtual ~DustProdLightStore();

	friend DustProdLightRef;
	friend DustProdLightEntity;
};

#endif /* DPL_IMPL_H_ */
