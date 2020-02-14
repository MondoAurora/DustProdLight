/*
 * fleetmandata.h
 *
 *  Created on: Feb 12, 2020
 *      Author: loran
 */

#include "dpl_impl.h"


	DustProdLightValue::~DustProdLightValue() {
	}
	;

	int DustProdLightValue::getInt() {
		return int_value;
	}

	double DustProdLightValue::getDouble() {
		return double_value;
	}

	string DustProdLightValue::getString() {
		return string_value;
	}

	void DustProdLightValue::set(DPLValType valType, void* pVal) {
		switch (valType) {
		case fmd_valtype_int:
			int_value = *((int*) pVal);
			break;
		case fmd_valtype_double:
			double_value = *((double*) pVal);
			break;
		case fmd_valtype_string:
			string_value = *((string*) pVal);
			break;
		default:
			throw DPLErrInvalidValueType();
		}
	}

	void DustProdLightEntity::updated() {
		changed = true;
	}

	void DustProdLightEntity::optReloadMeta() {
		if ( changed ) {
			types.clear();

			for(std::map<int, DustProdLightValue>::iterator iter = values.begin(); iter != values.end(); ++iter) {
				int k =  iter->first;
				types.insert(DustProdLightStore::store.vecTokens[k]->type);
			}
			for(std::map<int, DustProdLightRef*>::iterator iter = refs.begin(); iter != refs.end(); ++iter) {
				int k =  iter->first;
				types.insert(DustProdLightStore::store.vecTokens[k]->type);
			}
			changed = false;
		}
	}

	DustProdLightEntity::~DustProdLightEntity() {
	}

	DPLType DustProdLightEntity::getPrimaryType() {
		return primaryType;
	}

	bool DustProdLightEntity::isOfType(DPLType type) {
		return types.end() != types.find(type);
	}

	void DustProdLightEntity::getTypes(set<DPLType>& typeSet) {
		typeSet.clear();
		typeSet.insert(types.begin(), types.end());
	}

