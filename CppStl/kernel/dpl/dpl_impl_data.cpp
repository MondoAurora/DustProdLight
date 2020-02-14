/*
 * dpl_impl_data.h
 *
 *	Value and Entity implementation (not too complicated)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include "dpl_impl.h"


	DustProdLightValue::~DustProdLightValue() {
	}
	;

	void DustProdLightValue::set(DPLTokenType tokenType, void* pVal) {
		switch (tokenType) {
		case dpl_token_val_int:
			int_value = *((int*) pVal);
			break;
		case dpl_token_val_double:
			double_value = *((double*) pVal);
			break;
		case dpl_token_val_string:
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
				types.insert(DustProdLightStore::store->vecTokens[k]->type);
			}
			for(std::map<int, DustProdLightRef*>::iterator iter = refs.begin(); iter != refs.end(); ++iter) {
				int k =  iter->first;
				types.insert(DustProdLightStore::store->vecTokens[k]->type);
			}
			changed = false;
		}
	}

	DustProdLightEntity::~DustProdLightEntity() {
	}

	bool DustProdLightEntity::isOfType(DPLType type) {
		return types.end() != types.find(type);
	}

	void DustProdLightEntity::getTypes(set<DPLType>& typeSet) {
		typeSet.clear();
		typeSet.insert(types.begin(), types.end());
	}

