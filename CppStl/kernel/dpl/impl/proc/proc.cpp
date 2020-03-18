/*
 * dpluproc.cpp
 *
 * Process implementations
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#include <vector>

#include <iostream>
#include "proc.h"

using namespace std;

/****************************
 *
 * DustProdLightAgent
 *
 ****************************/

DustProdLightAgent::DustProdLightAgent() :
		pDialog(NULL), result(DPL_PROCESS_ACCEPT) {
}
DustProdLightAgent::~DustProdLightAgent() {

}

DPLProcessResult DustProdLightAgent::dplProcess() {
	return DPL_PROCESS_ACCEPT;
}

void DustProdLightAgent::init(DPLEntity eAgentDef, DustProdLightDialog *pDialog_) {
}
void DustProdLightAgent::step() {
}
void DustProdLightAgent::stepUp() {
}
void DustProdLightAgent::finish(bool error) {
}

/****************************
 *
 * DustProdLightDialog
 *
 ****************************/

DustProdLightDialog::DustProdLightDialog() {
}

DustProdLightDialog::~DustProdLightDialog() {

}

DPLProcessResult DustProdLightDialog::dplProcess() {
	return DPL_PROCESS_ACCEPT;
}

/****************************
 *
 * Call context management
 *
 ****************************/

DustProdLightEntity* DustProdLightImplementation::resolveEntity(DPLEntity entity) {
	return NULL;
}

/****************************
 *
 * Runtime
 *
 ****************************/
map<DPLEntity, const DPLModule*> DustProdLightRuntime::logicFactory;

DustProdLightProcess* DustProdLightRuntime::pProcessMain = NULL;
DustProdLightThread* DustProdLightRuntime::pThreadActive = NULL;

int DustProdLightRuntime::nextEntityId = DPL_MBI_;

map<string, int> DustProdLightRuntime::dataGlobal;

void DustProdLightRuntime::init() {
	if (!pProcessMain) {
		pProcessMain = new DustProdLightProcess(NULL);
		pThreadActive = new DustProdLightThread(pProcessMain);

		DustProdLightImplementation::createBootEntities();
	}

}

void DustProdLightRuntime::release() {
	if (pProcessMain) {
		delete pProcessMain;
		pProcessMain = NULL;

		delete pThreadActive;
		pThreadActive = NULL;
	}
}

/*******************************
 *
 * DustProdLightImplementation
 *
 *******************************/

string DustProdLightImplementation::getMetaEntityId(DPLTokenType tokenType, const char* name, DPLEntity parent) {
	switch (tokenType) {
	case DPL_ENTITY_INVALID:
		return name;
	case DPL_TOKEN_STORE: {
		string str = name;
		return str + DPL_SEP_STORE;
	}
	case DPL_TOKEN_UNIT:
		return getString(&DustProdLightRuntime::pProcessMain->emapHeap[DPL_MBI_STORE_SOURCE], DPL_MBI_ATT_ENTITY_GLOBALID)
				+ DPL_SEP_STORE + name;
	default:
		return
				parent ?
						getString(&DustProdLightRuntime::pProcessMain->emapHeap[parent], DPL_MBI_ATT_ENTITY_GLOBALID) + DPL_SEP_ID
								+ name :
						name;
	}

	return name;
}

void DustProdLightImplementation::initMetaEntity(DPLEntity entity, DPLTokenType tokenType, const char* name,
		DPLEntity parent) {
	DustProdLightEntity* pE = &DustProdLightRuntime::pProcessMain->emapHeap[entity];
	initMetaEntity(pE, entity, tokenType, name, parent);
}
/****************************
 *
 * DPL API
 *
 ****************************/

DPLEntity DPLData::getEntityById(const char* globalId) {
	map<string, int>::iterator i = DustProdLightRuntime::dataGlobal.find(globalId);
	return (DustProdLightRuntime::dataGlobal.end() == i) ? 0 : i->second;
}

DPLEntity DPLData::getMetaEntity(DPLTokenType tokenType, const char* name, DPLEntity parent) {
	DustProdLightRuntime::init();

	string id = DustProdLightImplementation::getMetaEntityId(tokenType, name, parent);
	int entity = DustProdLightRuntime::dataGlobal[id];

	if (!entity) {
		entity = ++DustProdLightRuntime::nextEntityId;

		DustProdLightRuntime::dataGlobal[id] = entity;
		DustProdLightEntity *pe = &DustProdLightRuntime::pProcessMain->emapHeap[entity];

		DustProdLightImplementation::initMetaEntity(pe, entity, tokenType, name, parent);
	}

	return entity;
}

DPLProcessResult DPLMain::send(DPLEntity target, DPLEntity command, DPLEntity param) {
	return DPL_PROCESS_REJECT;
}

void DPLMain::registerLogicProvider(const DPLModule *pLogicFactory, ...) {

	va_list args;
	va_start(args, pLogicFactory);
	int mId;

	while (DPL_ENTITY_INVALID != (mId = va_arg(args, int))) {
		DustProdLightRuntime::logicFactory[mId] = pLogicFactory;
	}

	va_end(args);
}
