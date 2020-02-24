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
	DustProdLightProcSession* pSession;

	int relayId = DPL_PROCESS_NO_ACTION;
	bool processed = true;

	int getRelay();

public:
	DustProdLightProcState(DustProdLightProcSession* pSession_) : pSession(pSession_) {};
	~DustProdLightProcState();

	virtual void setProcessed(bool p);

	virtual void* getContext(int ctxId);

	virtual DPLProcessResult requestRelay(int relayId_, bool processed_);

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
	DPLProcessAction *pProc;
	DustProdLightProcNodeDef *pNodeDef;
	DustProdLightProcSession *pSession = NULL;

	int nodeId = DPL_PROCESS_NO_ACTION;
	unsigned int pos = 0;
	unsigned int count = 0;
	bool inSep = false;

public:
	DustProdLightProcNode(DPLProcessAction *pProc);
	DustProdLightProcNode(DustProdLightProcNodeDef *pNodeDef_);
	~DustProdLightProcNode();

	void init(int nodeId_, DustProdLightProcSession *pSession_);
	DPLProcessResult process(DPLProcessState *pState);
	DPLProcessResult childReturned(DPLProcessResult childResult, DPLProcessState *pState);

	friend class DustProdLightProcEnv;
	friend class DustProdLightProcSession;
};


class DustProdLightProcSession : public DPLProcessImplementation {
	DustProdLightProcEnv *pEnv;

	stack<DustProdLightProcNode*> stack;

//	void* procCtx;
	map<int, void*> mapCtx;
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
	void* getContext(int ctxId);
	void finish(bool error);

	friend class DustProdLightProcEnv;
	friend class DustProc;
};


class DustProdLightProcEnv : public DPLProcessImplementation {
	static map<DPLProcessDefinition*, DustProdLightProcEnv*> environments;

	DPLProcessDefinition* pDef;
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
	static DustProdLightProcEnv* getEnv(DPLProcessDefinition *pProcDef);
	static void shutdown();

	DPLProcessResult executeProcess(void *pInitData);

	friend class DustProdLightProcSession;
	friend class DPLProc;
};




#endif /* DPL_IMPL_PROC_H_ */
