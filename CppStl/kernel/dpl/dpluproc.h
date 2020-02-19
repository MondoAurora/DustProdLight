/*
 * dpluproc.h
 *
 * Process services
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPLUPROC_H_
#define DPLUPROC_H_

#include <stack>
#include <map>
#include <set>

#include "dplutils.h"

using namespace std;

enum DPLUNodeTypes {
	DPLU_PROC_NODE_NULL, DPLU_PROC_NODE_SEQUENCE, DPLU_PROC_NODE_REPEAT, DPLU_PROC_NODE_SELECT,
};

class DPLUNodeDefinition {
	int id;

	DPLUNodeTypes nodeType;
	int separator;
	int members[];

	DPLUNodeDefinition();

public:
	static const DPLUNodeDefinition LIST_END;

	DPLUNodeDefinition(int nodeId, DPLUNodeTypes nodeType_, int separator_, int members_ ...);
};

class DPLUProcessEnvironment;

class DPLUProcessSession {
	DPLUProcessEnvironment *pEnv;

	stack<DPLProcessor*> relayStack;

	void* procCtx;
};



//template <class StaticContext, class DynamicContext>
class DPLUProcessEnvironment {
	map<int, set<DPLProcessor*>> processorPool;
	map<int, DPLUNodeDefinition> nodeDefs;

	set<DPLUProcessSession*> sessionPool;

	set<void*> procCtxPool;

protected:
	virtual ~DPLUProcessEnvironment();
	virtual DPLProcessor* createAtomicProcessor(int procId) = 0;
	virtual void* createProcessContext() = 0;

public:

};

class DPLUProcessorBase: public DPLProcessor {
protected:
	DPLProcessResult result;

public:
	virtual ~DPLUProcessorBase() {
	}
	virtual DPLProcessResult getProcessResult() {
		return result;
	}
};

class DPLUProcessorRelayNode: public DPLUProcessorBase {
public:
	virtual ~DPLUProcessorRelayNode() {
	}
	virtual bool isDynamic() = 0;
	virtual int getRelayType() = 0;
	virtual DPLUProcessorRelayNode* getRelayChild() = 0;
	virtual DPLProcessResult processRelayChildCompletion(DPLUProcessorRelayNode* pChild,
			DPLProcessResult childResult) = 0;
};

class DPLUProcessorRelayStack: public DPLUProcessorBase {
private:
	stack<DPLUProcessorRelayNode*> stack;

	void stepUp();
public:
	virtual ~DPLUProcessorRelayStack();

	void init(DPLUProcessorRelayNode* root);
	DPLProcessResult step();
	void finish();
};

#endif /* DPLUPROC_H_ */
