/*
 * dpl_impl_json.h
 *
 *  Created on: Feb 24, 2020
 *      Author: Lorand Kedves
 */

#include <iostream>
#include <fstream>

#include "dpl_impl_json.h"


const DPLEntity JsonMeta::DPLJsonUnit = DPLMeta::getUnit("TempJson");
const DPLEntity JsonMeta::DPLJsonTemp = DPLMeta::getType(JsonMeta::DPLJsonUnit, "TempJson");

const DPLEntity JsonMeta::DPLJsonCtxJson = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "CtxJson", DPL_TOKEN_ACTION);

const DPLEntity JsonMeta::DPLJsonMainRep = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "MainRep", DPL_TOKEN_ACTION);
const DPLEntity JsonMeta::DPLJsonMainSeq = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "MainSeq", DPL_TOKEN_ACTION);
const DPLEntity JsonMeta::DPLJsonCharSrc = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "CharSrc", DPL_TOKEN_ACTION);

const DPLEntity JsonMeta::DPLJsonValue = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "JsonValue", DPL_TOKEN_ACTION);

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
int JSON_LOGIC[] = {DPLJsonCtxJson, DPLJsonCharSrc, DPLJsonValue, DPL_PROCESS_NO_ACTION};

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
		DPLProc::registerLogicProvider(logicProvider);

		DPLProc::registerNarrative(DPLJsonTemp, jsonReader);

		DPLProc::registerCtrlRepeat(DPLJsonTemp, DPLJsonMainRep, DPLJsonMainSeq, 0, INT_MAX, DPL_PROCESS_NO_ACTION);
		DPLProc::registerCtrlSequence(DPLJsonTemp, DPLJsonMainSeq, DPL_PROCESS_NO_ACTION, DPLJsonCharSrc, DPLJsonValue, DPL_PROCESS_NO_ACTION);
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
	DPLProc::executeProcess(DPLJsonTemp, fileName);
}
