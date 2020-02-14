/*
 * fleetmandata.h
 *
 *  Created on: Feb 12, 2020
 *      Author: loran
 */

#ifndef DPL_H_
#define DPL_H_

#include <string>
#include <cstdarg>

using namespace std;

enum DPLValType {
	fmd_valtype_invalid,
	fmd_valtype_int,
	fmd_valtype_double,
	fmd_valtype_string,
	fmd_valtype_ref_single,
	fmd_valtype_ref_set,
	fmd_valtype_ref_array,
	fmd_valtype_ref_map
};

enum DPLChange {
	fmd_chg_ref_set, fmd_chg_ref_remove, fmd_chg_ref_clear,
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

class DPLValue {
public:
	virtual ~DPLValue() {
	}
	;

	virtual void set(DPLValType valType, void* pVal) = 0;

	virtual int getInt() = 0;
	virtual double getDouble() = 0;
	virtual string getString() = 0;
};

class DPLRef {
public:
	virtual ~DPLRef() {
	}
	;

	virtual DPLValType getType() = 0;

	virtual int getCount() = 0;
	virtual DPLEntity getRef(int key) = 0;
	virtual DPLToken getTokenByIndex(int idx) = 0;
};

class DPLEntityInt {
public:
	virtual ~DPLEntityInt() {
	}
	;

	virtual DPLType getPrimaryType() = 0;

	virtual bool isOfType(DPLType type) = 0;
	virtual void getTypes(set<DPLType>& typeSet) = 0;
};

class DPLStore {
protected:
	static DPLStore* pTheStore;

	virtual DPLType getTypeInt(string typeName) = 0;
	virtual DPLToken getTokenInt(DPLType type, string tokenName, DPLValType valType) = 0;

	virtual void setValue(DPLEntity entity, DPLToken token, DPLValType valType, void* pVal) = 0;
	virtual DPLValue *getValue(DPLEntity entity, DPLValType valType, DPLToken token) = 0;

	virtual DPLRef *getRef(DPLEntity entity, DPLToken token) = 0;

	virtual bool chgRef(DPLChange chg, DPLEntity entity, DPLToken token, DPLEntity target, int key) = 0;

	virtual DPLEntity createEntityInt(DPLType primaryType) = 0;
	virtual DPLEntityInt* getEntityInt(DPLEntity entity) = 0;

public:
	virtual ~DPLStore() {
	}
	;

	static DPLType getType(string typeName) {
		return pTheStore->getTypeInt(typeName);
	}

	static DPLToken getToken(DPLType type, string tokenName, DPLValType valType) {
		return pTheStore->getTokenInt(type, tokenName, valType);
	}

// int
	static void set_int(DPLEntity entity, DPLToken token, int i) {
		pTheStore->setValue(entity, token, fmd_valtype_int, &i);
	}

	static int get_int(DPLEntity entity, DPLToken token, int defValue) {
		DPLValue *pVal = pTheStore->getValue(entity, fmd_valtype_int, token);
		return (0 == pVal) ? defValue : pVal->getInt();
	}
// double
	static void set_double(DPLEntity entity, DPLToken token, double d) {
		pTheStore->setValue(entity, token, fmd_valtype_double, &d);
	}
	static double get_double(DPLEntity entity, DPLToken token, double defValue) {
		DPLValue *pVal = pTheStore->getValue(entity, fmd_valtype_double, token);
		return (0 == pVal) ? defValue : pVal->getDouble();
	}
// string
	static void set_string(DPLEntity entity, DPLToken token, string s) {
		pTheStore->setValue(entity, token, fmd_valtype_string, &s);

	}
	static string get_string(DPLEntity entity, DPLToken token, string defValue) {
		DPLValue *pVal = pTheStore->getValue(entity, fmd_valtype_string, token);
		return (0 == pVal) ? defValue : pVal->getString();
	}

// references
	static bool set_ref(DPLEntity entity, DPLToken token, DPLEntity target, int key) {
		return pTheStore->chgRef(fmd_chg_ref_set, entity, token, target, key);
	}

	static int get_ref_count(DPLEntity entity, DPLToken token) {
		DPLRef *pR = pTheStore->getRef(entity, token);
		return pR ? pR->getCount() : 0;
	}

	static DPLEntity get_ref(DPLEntity entity, DPLToken token, int key) {
		DPLRef *pR = pTheStore->getRef(entity, token);
		return pR ? pR->getRef(key) : 0;
	}

	static DPLToken get_ref_key(DPLEntity entity, DPLToken token, int idx) {
		DPLRef *pR = pTheStore->getRef(entity, token);
		return (pR && (fmd_valtype_ref_map == pR->getType())) ? pR->getTokenByIndex(idx) : 0;
	}

	static DPLEntity get_by_path(DPLEntity root, int path... ) {
		DPLEntity e = root;
		DPLRef *pR = NULL;

		va_list args;
		va_start(args, path);

		while (e) {
			int p = va_arg(args, int);

			if ( !p ) {
				break;
			}

			pR = pTheStore->getRef(e, p);

			if ( pR && pR->getCount() ) {
				switch ( pR->getType() ) {
					case fmd_valtype_ref_single:
					e = pR->getRef(0);
					break;
					case fmd_valtype_ref_set:
					case fmd_valtype_ref_array:
					p = va_arg(args, int);
					e = pR->getRef(p);
					break;
					case fmd_valtype_ref_map:
					p = va_arg(args, int);
					p = pR->getTokenByIndex(p);
					e = pR->getRef(p);
					break;
					default:
					e = 0;
					break;
				}
			} else {
				e = 0;
			}

			va_end(args);
		}

		return e;

	}

	static DPLEntity createEntity(DPLType primaryType) {
		return pTheStore->createEntityInt(primaryType);
	}

	static DPLType getPrimaryType(DPLEntity entity) {
		return pTheStore->getEntityInt(entity)->getPrimaryType();
	}
	static bool isOfType(DPLEntity entity, DPLType type) {
		return pTheStore->getEntityInt(entity)->isOfType(type);
	}
	static void getTypes(DPLEntity entity, set<DPLType>& typeSet)
	{
		pTheStore->getEntityInt(entity)->getTypes(typeSet);
	}

};

#endif /* DPL_H_ */
