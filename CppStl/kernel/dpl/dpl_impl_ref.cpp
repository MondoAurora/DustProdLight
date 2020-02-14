/*
 * fleetmandata.cpp
 *
 *  Created on: Feb 12, 2020
 *      Author: loran
 */

#include "dpl_impl.h"

DustProdLightRef::DustProdLightRef(DPLToken ptoken, DPLValType preftype, DPLEntity psource, DPLEntity ptarget,
		int pkey) :
		token(ptoken), refType(preftype), source(psource), target(ptarget) {
	if (fmd_valtype_ref_map == refType) {
		mapKey = pkey;
	}

	DustProdLightStore::store.refs.insert(this);
}

DustProdLightRef::DustProdLightRef(DustProdLightToken* ptoken, DPLEntity psource, DPLEntity ptarget, int pkey) :
		DustProdLightRef(ptoken->id, ptoken->valType, psource, ptarget, pkey) {
}

DustProdLightRef::DustProdLightRef(DustProdLightRef *porig, DPLEntity ptarget, int pkey) :
		DustProdLightRef(porig->token, porig->refType, porig->source, ptarget, pkey) {

	porig->append(this, pkey);
}

DustProdLightRef::~DustProdLightRef() {
	if (collection) {
		if (1 == collection->size()) {
			delete collection;
			DustProdLightStore::store.dataLocal[source].refs.erase(token);
		} else {
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				DustProdLightRef* pl = *it;
				if (pl == this) {
					collection->erase(it);
					break;
				}
			}

			DustProdLightRef *pEntry = DustProdLightStore::store.dataLocal[source].refs[token];
			if (pEntry == this) {
				pEntry = *collection->begin();
				DustProdLightStore::store.dataLocal[source].refs[token] = pEntry;
			}
		}
	}

	DustProdLightStore::store.refs.erase(this);
}

void DustProdLightRef::append(DustProdLightRef* pRef, int key) {
	if (!collection) {
		collection = new vector<DustProdLightRef*>();
		collection->push_back(this);
	}

	pRef->collection = collection;
	if ((fmd_valtype_ref_array == refType) && (0 <= key) && (key < (int) collection->size())) {
		collection->insert(collection->begin() + key, pRef);
	} else {
		collection->push_back(pRef);
	}
}

DustProdLightRef* DustProdLightRef::getBy(DPLEntity ptarget, int key) {
	if (collection) {
		DustProdLightRef *pL;
		switch (refType) {
		case fmd_valtype_ref_set:
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				pL = *it;
				if (pL->target == ptarget) {
					return pL;
				}
			}
			break;
		case fmd_valtype_ref_array:
			if ( REFKEY_ARR_APPEND != key) {
				pL = collection->at(key);
				if (pL->target == ptarget) {
					return pL;
				}
			}
			break;
		case fmd_valtype_ref_map:
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				pL = *it;
				if ((pL->target == ptarget) && (pL->mapKey == key)) {
					return pL;
				}
			}
			break;
		default:
			break;
		}
	} else {
		if (target == ptarget) {
			switch (refType) {
			case fmd_valtype_ref_map:
				if (mapKey != key) {
					return NULL;
				}
				break;
			case fmd_valtype_ref_array:
				if (0 != key) {
					return NULL;
				}
				break;
			default:
				break;
			}
			return this;
		}
	}

	return NULL;
}

DPLValType DustProdLightRef::getType() {
	return refType;
}

int DustProdLightRef::getCount() {
	return collection ? collection->size() : 1;
}

DPLEntity DustProdLightRef::getRef(int key) {
	DustProdLightRef *pL;

	if (fmd_valtype_ref_map == refType) {
		if (collection) {
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				pL = *it;
				if (pL->mapKey == key) {
					return pL->target;
				}
			}

		} else {
			return (mapKey == key) ? target : 0;
		}
	} else {
		if (collection) {
			if ((0 <= key) && (key < (int) collection->size())) {
				pL = *(collection->begin() + key);
				return pL->target;
			} else {
				return 0;
			}
		} else {
			return target;
		}
	}

	return 0;
}

DPLToken DustProdLightRef::getTokenByIndex(int idx) {
	if (fmd_valtype_ref_map == refType) {
		if (collection) {
			if ((0 <= idx) && (idx < (int) collection->size())) {
				DustProdLightRef* pL = *(collection->begin() + idx);
				return pL->mapKey;
			} else {
				return 0;
			}
		} else {
			return (idx) ? 0 : mapKey;
		}

	}

	return 0;
}
