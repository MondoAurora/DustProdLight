/*
 * dpl_impl_json.h
 *
 *  Created on: Feb 24, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_JSON_H_
#define DPL_IMPL_JSON_H_

#include "dplujson.h"
#include "dpl_impl.h"

namespace JsonMeta {
extern const DPLEntity DPLJsonUnit;

extern const DPLEntity DPLJsonTemp;
extern const DPLEntity DPLJsonCtxJson;

extern const DPLEntity DPLJsonMainRep;
extern const DPLEntity DPLJsonMainSeq;
extern const DPLEntity DPLJsonCharSrc;

extern const DPLEntity DPLJsonValue;
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

	char c;
	bool charRead;
	int pos;
};

class DPLUActionDump: public DPLProcessAction {
public:
	virtual ~DPLUActionDump() {
	}
	virtual DPLProcessResult dplProcess(DPLProcessState *pState);
};

class DPLUActionStreamReader: public DPLProcessAction {
	ifstream inStream;

public:
	virtual ~DPLUActionStreamReader() {
	}
//	virtual void dplInit(DPLProcessState *pState);
	virtual DPLProcessResult dplProcess(DPLProcessState *pState);
//	virtual void dplRelease(DPLProcessState *pState);
};

class DPLJsonReader: public DPLProcessDefinition {
public:
	virtual ~DPLJsonReader();

	virtual int getStartNode();

	virtual void openProcessContext(int ctxId, void* pCtx, const void *pData);
};

class DPLJsonLogicProvider: public DPLLogicProvider {
public:
	DPLJsonLogicProvider(int *ids_) : DPLLogicProvider(ids_) {}
	virtual ~DPLJsonLogicProvider();
	virtual void* createLogic(int logicId);
	virtual void releaseLogic(int logicId, void* pLogic);
};

#endif /* DPL_IMPL_JSON_H_ */
