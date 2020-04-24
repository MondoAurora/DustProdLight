/*
 * dpl_impl_ref.cpp
 *
 *	Entity reference implementation (quite complicated, got a separate file)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include "data.h"
#include "proc_runtime.h"

DustProdLightRef::DustProdLightRef(const DustProdLightRef& r) :
		DustProdLightRef(r.token, r.tokenType, r.source, r.target, r.mapKey) {
	if (r.collection) {
		int s = r.collection->size();
		if (s > 1) {
			collection = new vector<DustProdLightRef*>();
			for (int i = 0; i < s; ++i) {
				DustProdLightRef *p = (*r.collection)[i];
				collection->push_back( (p == &r) ? this:
						new DustProdLightRef(token, tokenType, source, p->target, i));
			}
		}
	}
}

DustProdLightRef::DustProdLightRef(DPLEntity ptoken, DPLTokenType ptokentype, DPLEntity psource, DPLEntity ptarget,
		int pkey) :
		token(ptoken), tokenType(ptokentype), source(psource), target(ptarget) {
	if (DPL_TOKEN_REF_MAP == tokenType) {
		mapKey = pkey;
	}
}

DustProdLightRef::DustProdLightRef(DustProdLightEntity* ptoken, DPLEntity psource, DPLEntity ptarget, int pkey) :
		DustProdLightRef(ptoken->localId, (DPLTokenType) ptoken->primaryType, psource, ptarget, pkey) {
}

DustProdLightRef::DustProdLightRef(DustProdLightRef *porig, DPLEntity ptarget, int pkey) :
		DustProdLightRef(porig->token, porig->tokenType, porig->source, ptarget, pkey) {

	porig->append(this, pkey);
}

DustProdLightRef::~DustProdLightRef() {
	if (collection) {
		if (1 == collection->size()) {
			delete collection;
		} else {
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				DustProdLightRef* pl = *it;
				if (pl == this) {
					collection->erase(it);
					break;
				}
			}

			DustProdLightEntity* pE = DustProdLightRuntime::pRuntime->resolveEntity(source);
			DustProdLightRef *pEntry = pE->refs[token];
			if (pEntry == this) {
				pEntry = *collection->begin();
				pE->refs[token] = pEntry;
			}
		}
	}
}

void DustProdLightRef::append(DustProdLightRef* pRef, int key) {
	if (!collection) {
		collection = new vector<DustProdLightRef*>();
		collection->push_back(this);
	}

	pRef->collection = collection;
	if ((DPL_TOKEN_REF_ARR == tokenType) && (0 <= key) && (key < (int) collection->size())) {
		collection->insert(collection->begin() + key, pRef);
	} else {
		collection->push_back(pRef);
	}
}

DustProdLightRef* DustProdLightRef::getBy(DPLEntity ptarget, int key) {
	if (collection) {
		DustProdLightRef *pL;
		switch (tokenType) {
		case DPL_TOKEN_REF_SET:
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				pL = *it;
				if (pL->target == ptarget) {
					return pL;
				}
			}
			break;
		case DPL_TOKEN_REF_ARR:
			if ( DPL_REFKEY_ARR_APPEND != key) {
				pL = collection->at(key);
				if (pL->target == ptarget) {
					return pL;
				}
			}
			break;
		case DPL_TOKEN_REF_MAP:
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
			switch (tokenType) {
			case DPL_TOKEN_REF_MAP:
				if (mapKey != key) {
					return NULL;
				}
				break;
			case DPL_TOKEN_REF_ARR:
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

int DustProdLightRef::getCount() {
	return collection ? collection->size() : 1;
}

DPLEntity DustProdLightRef::getRef(int key) {
	DustProdLightRef *pL;

	if (DPL_TOKEN_REF_MAP == tokenType) {
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

DPLEntity DustProdLightRef::getTokenByIndex(int idx) {
	if (DPL_TOKEN_REF_MAP == tokenType) {
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

void DustProdLightRef::doVisit(DPLVisitor *pVisitor, int key, void *pHint, DPLProcessResult fr) {
	switch (fr) {
	case DPL_PROCESS_READ:
		pVisitor->processBeginEntity(target, key, pHint);
		pVisitor->processEndEntity(target, key, pHint);
		break;
	case DPL_PROCESS_ACCEPT_READ:
		DustProdLightRuntime::pRuntime->resolveEntity(target)->optVisit(pVisitor, key, pHint);
		break;
	default:
		break;
	}
}

void DustProdLightRef::optVisit(DPLVisitor *pVisitor, void *pHint) {
	DPLProcessResult fr = pVisitor->shouldProcess(source, token);

	if (DPL_PROCESS_ACCEPT_PASS != fr) {
		pVisitor->processRefBegin(source, token, tokenType, pHint);

		if (collection) {
			int key = 0;
			for (RefVectorIterator it = collection->begin(); it != collection->end(); ++it) {
				DustProdLightRef *pRef = *it;
				pRef->doVisit(pVisitor, (DPL_TOKEN_REF_MAP == tokenType) ? pRef->mapKey : key++, pHint, fr);
			}
		} else {
			doVisit(pVisitor, (DPL_TOKEN_REF_MAP == tokenType) ? mapKey : 0, pHint, fr);
		}

		pVisitor->processRefEnd(source, token, tokenType, pHint);
	}

}

