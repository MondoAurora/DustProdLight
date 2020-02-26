/*
 * dpl_impl_json.h
 *
 *  Created on: Feb 24, 2020
 *      Author: Lorand Kedves
 */

#include <iostream>
#include <fstream>

#include "dpl_impl_json.h"


const DPLType JsonMeta::DPLJsonTemp = DPLMeta::getType("TempJson");

const DPLToken JsonMeta::DPLJsonCtxJson = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "CtxJson", DPL_TOKEN_ACTION);

const DPLToken JsonMeta::DPLJsonMainRep = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "MainRep", DPL_TOKEN_ACTION);
const DPLToken JsonMeta::DPLJsonMainSeq = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "MainSeq", DPL_TOKEN_ACTION);
const DPLToken JsonMeta::DPLJsonCharSrc = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "CharSrc", DPL_TOKEN_ACTION);

const DPLToken JsonMeta::DPLJsonValue = DPLMeta::getToken(JsonMeta::DPLJsonTemp, "JsonValue", DPL_TOKEN_ACTION);

using namespace JsonMeta;


DPLProcessResult DPLUActionStreamReader::dplProcess(DPLProcessState *pState) {
	DPLUProcCtxJson* pCtx = (DPLUProcCtxJson*) pState->getContext(DPLJsonCtxJson);

	pCtx->inStream.get(pCtx->c);
	pCtx->charRead = pCtx->inStream.good();

	return DPL_PROCESS_SUCCESS;
}

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
		DPLUProcCtxJson* pc = (DPLUProcCtxJson*) pCtx;
		pc->fName = (const char*) pData;
		pc->inStream.open(pc->fName);
	}
		break;
	default:
		break;
	}
}

void DPLJsonReader::closeProcessContext(int ctxId, void* pCtx) {
	switch (logicProvider->indexOf(ctxId)) {
	case 0: {
		DPLUProcCtxJson* pc = (DPLUProcCtxJson*) pCtx;
		pc->inStream.close();
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
