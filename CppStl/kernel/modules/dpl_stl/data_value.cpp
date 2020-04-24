/*
 * dpl_impl_data.h
 *
 *	Value and Entity implementation (not too complicated)
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include "data.h"

DustProdLightValue::~DustProdLightValue() {
}
;

void DustProdLightValue::set(DPLTokenType tokenType, const void* pVal) {
	this->tokenType = tokenType;

	switch (tokenType) {
	case DPL_TOKEN_VAL_BOOL:
		valInt = 0 != *((int*) pVal);
		break;
	case DPL_TOKEN_VAL_INT:
		valInt = *((int*) pVal);
		break;
	case DPL_TOKEN_VAL_DOUBLE:
		valDbl = *((double*) pVal);
		break;
	case DPL_TOKEN_VAL_STRING:
		valStr = *((string*) pVal);
		break;
	default:
		throw DPLError(DPL_ERR_UNSPECIFIED);
	}
}

void DustProdLightValue::optVisit(DPLVisitor *pVisitor, DPLEntity entity, DPLEntity token, void *pHint) {
	if (DPL_PROCESS_REJECT != pVisitor->shouldProcess(entity, token)) {
		switch (tokenType) {
		case DPL_TOKEN_VAL_BOOL:
			pVisitor->processValBool(entity, token, valInt, pHint);
			break;
		case DPL_TOKEN_VAL_INT:
			pVisitor->processValInt(entity, token, valInt, pHint);
			break;
		case DPL_TOKEN_VAL_DOUBLE:
			pVisitor->processValDouble(entity, token, valDbl, pHint);
			break;
		case DPL_TOKEN_VAL_STRING:
			pVisitor->processValString(entity, token, valStr, pHint);
			break;
		default:
			throw DPLError(DPL_ERR_UNSPECIFIED);
		}
	}
}
