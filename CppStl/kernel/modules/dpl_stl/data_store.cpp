#include "data.h"
#include "_dplgen_boot.h"

DustProdLightStore::DustProdLightStore()
:lastEntityId(DPL_MBI_)
{
}

DustProdLightEntity *DustProdLightStore::resolve(DPLEntity e) {
	DustProdLightEntity* ret = NULL;
	switch ( e ) {
	case DPL_ENTITY_APPEND:
		ret = &emapLocal[++lastEntityId];
		ret->localId = lastEntityId;
		break;
	case DPL_ENTITY_INVALID:
		ret = NULL;
		break;
	default:
		if ( (0 < e) && (e <= lastEntityId) ) {
			ret = &emapLocal[e];
		}
		break;
	}

	return ret;
}
