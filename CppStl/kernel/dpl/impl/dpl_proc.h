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

#include "dpl_boot.h"
#include "dpl_meta.h"
#include "dpl_data.h"

using namespace std;

class DustProdLightDialogTokenRing;
class DustProdLightAgent;

class DPLUActionDump: public DPLAction {
public:
	virtual ~DPLUActionDump() {
	}
	virtual DPLProcessResult dplProcess();
};

class DPLUActionStreamReader: public DPLAction {
	ifstream inStream;
	int pos;

public:
	virtual ~DPLUActionStreamReader() {
	}
	virtual DPLProcessResult dplProcess();
	virtual void dplRelease();
};

class ProcActionSignal: public DPLAction {
public:
	virtual ~ProcActionSignal() {
	}
	virtual DPLProcessResult dplProcess();
};

class ProcActionControl: public DPLAction {
public:
	virtual ~ProcActionControl() {
	}
};

class ProcActionSequence: public ProcActionControl {
	unsigned int pos = 0;
	bool inSep = false;

public:
	virtual ~ProcActionSequence() {
	}
	virtual DPLProcessResult dplProcess();
	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse);
};

class ProcActionSelect: public ProcActionControl {
	unsigned int pos = 0;

public:
	virtual ~ProcActionSelect() {
	}
	virtual DPLProcessResult dplProcess();
	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse);
};

class ProcActionRepeat: public ProcActionControl {
	unsigned int count = 0;
	bool inSep = false;
	unsigned int min = 0;
	unsigned int max = INT_MAX;

public:
	virtual ~ProcActionRepeat() {
	}
	virtual DPLProcessResult dplProcess();
	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse);
};

typedef map<int, DustProdLightEntity*>::iterator EntityPtrIterator;
typedef map<int, DustProdLightEntity>::iterator EntityIterator;

class DustProdLightProcess {
	DustProdLightProcess *pParent;
	map<int, DustProdLightEntity> emapHeap;
//	int nextTempId = -1;

public:
	DustProdLightProcess() :
			DustProdLightProcess(NULL) {
	}
	DustProdLightProcess(DustProdLightProcess *pParent_) :
			pParent(pParent_) {
	}
	~DustProdLightProcess() {
	}

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
};

class DustProdLightThread {
	DustProdLightProcess *pProcess;

	DustProdLightAgent* pAgent = NULL;
	volatile bool requestSuspend = false;
	DPLProcessResult result = DPL_PROCESS_ACCEPT;

public:
	DustProdLightThread(DustProdLightProcess *pProcess_) :
			pProcess(pProcess_) {
	}
	~DustProdLightThread() {
	}
	;

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
};

class DustProdLightBlock {
private:
	DustProdLightProcess *pProcess;

	DPLBlock blockType;
	map<int, DustProdLightEntity*> emapRef;
	map<int, DustProdLightEntity> emapLocal;

	map<int, DustProdLightEntity> emapMsg;

	DPLAction* pAction;

public:
	void release();

	DPLEntity getMsgEntity(DPLEntity cmd, DPLEntity target);
	DustProdLightEntity* getEntity(DPLEntity e) ;

	void exec(DustProdLightEntity *pmsg);

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
	virtual ~DustProdLightAgent();

	DustProdLightBlock* init(DPLEntity eAgentStart, map<int, DustProdLightEntity> *pHeap);
	DustProdLightBlock* getCurrBlock() {
		return &stack[stackPos];
	}

	virtual DPLProcessResult dplProcess();
};

class DustProdLightDialogTokenRing: public DPLAction {
	vector<DustProdLightAgent*> agents;
	int currAgent;

public:
	DustProdLightDialogTokenRing();
	virtual ~DustProdLightDialogTokenRing();

	virtual DPLProcessResult dplProcess();

	friend class DPLMain;
	friend class DPLProc;
};

class DustProdLightRuntime {
	static DustProdLightRuntime *pRuntime;
	static DustProdLightEntity *pRefMsgCmd;
	static DustProdLightEntity *pRefMsgTarget;

	map<DPLEntity, DPLModule*> logicFactory;

	DustProdLightProcess processMain;
	DustProdLightThread threadSingle;
	DustProdLightAgent agentMain;

	DustProdLightThread *pThreadActive;

	int nextEntityId;
	map<string, int> dataGlobal;

	DustProdLightRuntime();
	~DustProdLightRuntime();

public:
	static void init();
	static void release();

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
