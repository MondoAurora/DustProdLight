/*
 * dpl_impl_json.h
 *
 *  Created on: Feb 24, 2020
 *      Author: Lorand Kedves
 */

#include <iostream>
#include <fstream>

#include "dpl_impl_json.h"


const DPLEntity JsonMeta::DPLJsonUnit = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "TempJson");
const DPLEntity JsonMeta::DPLJsonTemp = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "TempJson", JsonMeta::DPLJsonUnit);

const DPLEntity JsonMeta::DPLJsonCtxJson = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "CtxJson", JsonMeta::DPLJsonTemp);

const DPLEntity JsonMeta::DPLJsonMainRep = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "MainRep", JsonMeta::DPLJsonTemp);
const DPLEntity JsonMeta::DPLJsonMainSeq = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "MainSeq", JsonMeta::DPLJsonTemp);
const DPLEntity JsonMeta::DPLJsonCharSrc = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "CharSrc", JsonMeta::DPLJsonTemp);

const DPLEntity JsonMeta::DPLJsonValue = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "JsonValue", JsonMeta::DPLJsonTemp);

using namespace JsonMeta;


//void DPLUActionStreamReader::dplInit(DPLProcessState *pState) {
//	DPLUProcCtxJson* pCtx = (DPLUProcCtxJson*) pState->getContext(DPLJsonCtxJson);
//
//	if ( !inStream.is_open() ) {
//		inStream.open(pCtx->fName);
//	}
//}

DPLProcessResult DPLUActionStreamReader::dplProcess(DPLProcessState *pState) {
	DPLUProcCtxJson* pCtx = (DPLUProcCtxJson*) pState->getContext(DPLJsonCtxJson);

	if ( !inStream.is_open() ) {
		inStream.open(pCtx->fName);
		pCtx->pos = 0;
	}

	inStream.get(pCtx->c);
	pCtx->charRead = inStream.good();

	if ( pCtx->charRead ) {
		++pCtx->pos;
	} else {
		inStream.close();
	}


	return DPL_PROCESS_SUCCESS;
}

//void DPLUActionStreamReader::dplRelease(DPLProcessState *pState) {
//	if ( inStream.is_open() ) {
//		inStream.close();
//	}
//}

DPLProcessResult DPLUActionDump::dplProcess(DPLProcessState *pState) {
	DPLUProcCtxJson* pCtx = (DPLUProcCtxJson*) pState->getContext(DPLJsonCtxJson);

	if (pCtx->charRead) {
		cout << pCtx->c;
		return DPL_PROCESS_SUCCESS;
	} else {
		return DPL_PROCESS_REJECT;
	}
}

DPLJsonReader::~DPLJsonReader() {

}

int DPLJsonReader::getStartNode() {
	return DPLJsonMainRep;
}

DPLJsonLogicProvider *logicProvider = NULL;
int JSON_LOGIC[] = {DPLJsonCtxJson, DPLJsonCharSrc, DPLJsonValue, DPL_ENTITY_INVALID};

DPLJsonLogicProvider::~DPLJsonLogicProvider() {

}

void* DPLJsonLogicProvider::createLogic(int logicId) {
	switch (indexOf(logicId)) {
	case 0:
		return new DPLUProcCtxJson();
	case 1:
		return new DPLUActionStreamReader();
	case 2:
		return new DPLUActionDump();
	default:
		return NULL;
	}
}

void DPLJsonLogicProvider::releaseLogic(int logicId, void* pLogic) {
	switch (indexOf(logicId)) {
	case 0:
		delete (DPLUProcCtxJson*) pLogic;
		break;
	case 1:
		delete (DPLUActionStreamReader*) pLogic;
		break;
	case 2:
		delete (DPLUActionDump*) pLogic;
		break;
	default:
		break;
	}
}

void DPLJsonReader::openProcessContext(int ctxId, void* pCtx, const void *pData) {
	switch (logicProvider->indexOf(ctxId)) {
	case 0: {
		((DPLUProcCtxJson*)pCtx)->fName = (const char*) pData;
	}
		break;
	default:
		break;
	}
}

DPLJsonReader jsonReader;


void DPLJson::init() {
	if (!logicProvider) {
		logicProvider = new DPLJsonLogicProvider(JSON_LOGIC);
		DPLMain::registerLogicProvider(logicProvider);

//		DPLProc::registerNarrative(DPLJsonTemp, jsonReader);
//
//		DPLProc::registerCtrlRepeat(DPLJsonTemp, DPLJsonMainRep, DPLJsonMainSeq, 0, INT_MAX, DPL_PROCESS_NO_ACTION);
//		DPLProc::registerCtrlSequence(DPLJsonTemp, DPLJsonMainSeq, DPL_PROCESS_NO_ACTION, DPLJsonCharSrc, DPLJsonValue, DPL_PROCESS_NO_ACTION);
	}
}
void DPLJson::shutdown() {
	if (!logicProvider) {
		delete logicProvider;
		logicProvider = NULL;
	}
}
void DPLJson::read(const char* fileName) {
	init();

//	DPLProc::executeProcess(DPLJsonTemp, fileName);
}
