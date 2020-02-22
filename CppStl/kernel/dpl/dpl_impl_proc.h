/*
 * dpl_impl_proc.h
 *
 * Process services
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_PROC_H_
#define DPL_IMPL_PROC_H_

#include <stack>
#include <map>
#include <set>
#include <vector>
#include <array>

#include "dplutils.h"
#include "dpl_impl_proc.h"

using namespace std;

class DustProdLightProcEnv;
class DustProdLightProcSession;
class DustProdLightProcNode;

enum DPLProcessNodeTypes {
	DPLU_PROC_NODE_NULL, DPLU_PROC_NODE_SEQUENCE, DPLU_PROC_NODE_REPEAT, DPLU_PROC_NODE_SELECT,
};


class DustProdLightProcState : public DPLProcessState {
private:
	int relayId = DPL_PROCESS_NO_RELAY;
	bool processed = true;

	int getRelay() {
		int ret = relayId;
		relayId = DPL_PROCESS_NO_RELAY;
		return ret;
	}

public:
	virtual void setProcessed(bool p) {
		processed = p;
	}

	virtual DPLProcessResult requestRelay(int relayId_, bool processed_) {
		relayId = relayId_;
		processed = processed_;
		return DPL_PROCESS_RELAY;
	}

	friend class DustProdLightProcSession;
	friend class DustProdLightProcNode;
};

class DustProdLightProcNodeDef {
	int id;

	DPLProcessNodeTypes nodeType;
	vector<int> members;
	int separator;

	int rep;
	unsigned int min;
	unsigned int max;

	DustProdLightProcNodeDef();

public:
	static const DustProdLightProcNodeDef LIST_END;

	DustProdLightProcNodeDef(int nodeId, DPLProcessNodeTypes nodeType_, int separator_, vector<int> members_);
	DustProdLightProcNodeDef(int nodeId, int rep_, int min_, int max_, int separator_);

	friend class DustProdLightProcEnv;
	friend class DustProdLightProcNode;
};


class DustProdLightProcNode : public DPLProcessImplementation {
	DPLProcessor *pProc;
	DustProdLightProcNodeDef *pNodeDef;
	DustProdLightProcSession *pSession = NULL;

	int nodeId = DPL_PROCESS_NO_RELAY;
	unsigned int pos = 0;
	unsigned int count = 0;
	bool inSep = false;

public:
	DustProdLightProcNode(DPLProcessor *pProc);
	DustProdLightProcNode(DustProdLightProcNodeDef *pNodeDef_);
	~DustProdLightProcNode();

	void init(int nodeId_, DustProdLightProcSession *pSession_);
	DPLProcessResult process(void *pCtx, DPLProcessState *pState);
	DPLProcessResult childReturned(void *pCtx, DPLProcessResult childResult, DPLProcessState *pState);

	friend class DustProdLightProcEnv;
	friend class DustProdLightProcSession;
};


class DustProdLightProcSession : public DPLProcessImplementation {
	DustProdLightProcEnv *pEnv;

	stack<DustProdLightProcNode*> stack;

	void* procCtx;
	DustProdLightProcState state;
	DPLProcessResult result;

	DustProdLightProcNode* selectNode(int nodeId) ;

	DPLProcessResult step();
	void walkUp();
	void walkDown();

public:
	DustProdLightProcSession(DustProdLightProcEnv *pEnv_);
	virtual ~DustProdLightProcSession();

	void open(void* pInitData);
	void finish(bool error);

	friend class DustProdLightProcEnv;
	friend class DustProc;
};


class DustProdLightProcEnv : public DPLProcessImplementation {
	static map<DPLProcessDef*, DustProdLightProcEnv*> environments;

	DPLProcessDef* pDef;
	map<int, DustProdLightProcNodeDef*> ctrlNodeDefs;

	map<int, set<DustProdLightProcNode*>> processorPool;

	set<DustProdLightProcSession*> sessionPool;

protected:
	virtual ~DustProdLightProcEnv();

	DustProdLightProcSession* getSession(void *pInitData);
	void releaseSession(DustProdLightProcSession *pSession);

	DustProdLightProcNode* getProcessor(int procId);
	void releaseProcessor(DustProdLightProcNode* proc);

public:
	static DustProdLightProcEnv* getEnv(DPLProcessDef *pProcDef);
	static void shutdown();

	DPLProcessResult executeProcess(void *pInitData);

	friend class DustProdLightProcSession;
	friend class DPLProc;
};




#endif /* DPL_IMPL_PROC_H_ */
