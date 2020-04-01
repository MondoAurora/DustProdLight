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
#include <fstream>

#include "data.h"

using namespace std;

typedef map<int, DustProdLightEntity*>::iterator EntityPtrIterator;
typedef map<int, DustProdLightEntity>::iterator EntityIterator;

class DustProdLightBlock {
private:
	DPLBlock blockType;
	map<int, DustProdLightEntity*> emapRef;
	map<int, DustProdLightEntity> emapLocal;

	map<int, DustProdLightEntity> emapMsg;

	map<DPLEntity, DPLAction*> actionByCmd;

public:
	DPLEntity getMsgEntity(DPLEntity cmd, DPLEntity target);
	DustProdLightEntity* getEntity(DPLEntity e) ;

	void init(DustProdLightEntity *pmsg);
	DPLProcessResult exec(DPLEntity cmd);
	void release();

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
	friend class DustProdLightAgent;

};

class DustProdLightAgent: public DPLAction {
	map<int, DustProdLightEntity> *pHeap = NULL;

	map<int, DustProdLightBlock> stack;
	int stackPos = 0;

	DPLProcessResult result = DPL_PROCESS_ACCEPT;

	void step();
	void stepUp();
	void finish(bool error);

public:
	DustProdLightAgent();
	virtual ~DustProdLightAgent();

	DustProdLightBlock* init(DPLEntity eAgentStart, map<int, DustProdLightEntity> *pHeap);
	DustProdLightBlock* getCurrBlock() {
		return &stack[stackPos];
	}

	virtual DPLProcessResult dplProcess();

	friend class DustProdLightRuntime;
};

class DustProdLightThread {
	DustProdLightAgent* pAgent = NULL;
	volatile bool requestSuspend = false;
	DPLProcessResult result = DPL_PROCESS_ACCEPT;

public:
	~DustProdLightThread() {
	}
	;

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
};

class DustProdLightRuntime {
	static DustProdLightRuntime *pRuntime;
	static DustProdLightEntity *pRefMsgCmd;
	static DustProdLightEntity *pRefMsgTarget;

	map<DPLEntity, DPLModule*> logicFactory;

	DustProdLightThread threadSingle;
	DustProdLightAgent agentMain;

	DustProdLightThread *pThreadActive;

	int nextEntityId;
	map<string, int> dataGlobal;

	DustProdLightRuntime();
	~DustProdLightRuntime();

public:
	static DustProdLightEntity *getRootEntity(DPLEntity entity);

	static void init();
	static void release();

	static DPLAction *createAction(DPLEntity eAction);
	static void releaseAction(DPLEntity eAction, DPLAction *pAction);

	DPLEntity resolveCtxEntity(DPLContext ctx);
	DustProdLightEntity *resolveEntity(DPLEntity e);

	static string getMetaEntityId(DPLTokenType tokenType, string name, DPLEntity parent);
	static void initMetaEntity(DPLEntity entity, DPLTokenType tokenType, string name, DPLEntity parent =
			DPL_ENTITY_INVALID);

	void validateToken(DPLEntity token, DPLTokenType tokenType);
	DustProdLightValue* getValue(DPLEntity entity, DPLTokenType tokenType, DPLEntity token);
	void setValue(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void* pVal);

	bool chgRef(DPLChange chg, DPLEntity entity, DPLEntity token, DPLEntity target, int key);
	DustProdLightRef* getRef(DPLEntity entity, DPLEntity token);
	DustProdLightEntity *createEntity(DPLEntity primaryType);

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightEntity;
	friend class DustProdLightRef;
	friend class DustProdLightBlock;
};

#endif /* DPL_IMPL_PROC_H_ */
