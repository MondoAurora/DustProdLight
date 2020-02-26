/*
 * dpl_impl_json.h
 *
 *  Created on: Feb 24, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_JSON_H_
#define DPL_IMPL_JSON_H_

#include "dplujson.h"

namespace JsonMeta {
extern const DPLType DPLJsonTemp;
extern const DPLToken DPLJsonCtxJson;

extern const DPLToken DPLJsonMainRep;
extern const DPLToken DPLJsonMainSeq;
extern const DPLToken DPLJsonCharSrc;

extern const DPLToken DPLJsonValue;
}

//class DPLUProcCtxFile {
//
//};
//
//class DPLUProcCtxChar {
//
//};

class DPLUProcCtxJson {
public:
	const char* fName;
	ifstream inStream;

	bool charRead;
	int pos;
	char c;
};

class DPLUActionDump: public DPLProcessAction {
public:
	virtual ~DPLUActionDump() {
	}
	;
	virtual DPLProcessResult dplProcess(DPLProcessState *pState);
};

class DPLUActionStreamReader: public DPLProcessAction {
public:
	virtual ~DPLUActionStreamReader() {
	}
	;
	virtual DPLProcessResult dplProcess(DPLProcessState *pState);
};

class DPLJsonReader: public DPLProcessDefinition {
public:
	virtual ~DPLJsonReader();

	virtual int getStartNode();

	virtual void openProcessContext(int ctxId, void* pCtx, const void *pData);
	virtual void closeProcessContext(int ctxId, void* pCtx);
};

class DPLJsonLogicProvider: public DPLLogicProvider {
public:
	DPLJsonLogicProvider(int *ids_) : DPLLogicProvider(ids_) {}
	virtual ~DPLJsonLogicProvider();
	virtual void* createLogic(int logicId);
	virtual void releaseLogic(int logicId, void* pLogic);
};

#endif /* DPL_IMPL_JSON_H_ */
