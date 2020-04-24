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

class DustProdLightAgent;

class DustProdLightBlock: public DPLNarrativeLogic {
private:
	map<int, DustProdLightEntity*> emapRef;
	DustProdLightStore *pStore;

	DPLNarrativeLogic* pLogic;

public:
	DustProdLightBlock();
	~DustProdLightBlock();

	DustProdLightEntity* getEntity(DPLEntity e) ;

	DPLProcessResult init(DustProdLightEntity *pSelf, DustProdLightBlock *pParent);

	virtual DPLProcessResult dplActionExecute();
	virtual DPLProcessResult dplResourceRelease();

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
	friend class DustProdLightAgent;
	friend class ProcActionControl;
};

typedef map<DPLEntity, DustProdLightBlock*>::const_iterator BlockIterator;

class DustProdLightAgent: public DPLNarrativeLogic {
	map<int, DustProdLightBlock*> stack;
	int stackPos = -1;

public:
	DustProdLightAgent();
	virtual ~DustProdLightAgent();

	virtual DPLProcessResult dplActionExecute();
	virtual DPLProcessResult dplResourceRelease();

	DustProdLightBlock* getBlock() {
		return stack[stackPos];
	}

	void relayEntry(DustProdLightBlock *pBlockRelay);
	void relayExit();

	DustProdLightEntity* resolveEntity(DPLEntity entity) {
		DustProdLightBlock* pB = stack[stackPos];
		return pB ? pB->getEntity(entity) : NULL;
	}

	friend class DustProdLightRuntime;
	friend class ProcActionControl;
	friend class DustProdLightDialog;
};

class DustProdLightDialog : public DPLNarrativeLogic {
	vector<DustProdLightAgent> agents;
	unsigned int currAgent = 0;
	DustProdLightEntity *pData;

public:
	DustProdLightDialog();
	~DustProdLightDialog();

	DustProdLightAgent* getAgent() {
		return &agents[currAgent];
	}

	DustProdLightAgent* getAgent(unsigned int idx) {
		if ( idx >= agents.size() ) {
			agents.resize(idx+1);
		}

		return &agents[idx];
	}

	void getData(DustProdLightEntity* pData_) {
		pData = pData_;
	}

	DustProdLightEntity* getData() {
		return pData;
	}

	virtual DPLProcessResult dplActionExecute();
	virtual DPLProcessResult dplResourceRelease();

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
};

class DustProdLightCore : public DPLNarrativeLogic {
	DustProdLightDialog *pDialog = NULL;
	DPLProcessResult lastResult;

public:
	DPLProcessResult getLastResult () {
		return lastResult;
	}
	DustProdLightDialog* getDialog() {
		return pDialog;
	}

	DustProdLightEntity* resolveEntity(DPLEntity entity);

	virtual DPLProcessResult dplActionExecute();

	DPLProcessResult run(int dlgIdx, DustProdLightBlock *pBlock, int agentCount = 1);

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightBlock;
	friend class DustProdLightAgent;
	friend class DustProdLightRuntime;
	friend class ProcActionControl;
};

class DustProdLightRuntime {
	static DustProdLightRuntime *pRuntime;

	map<DPLEntity, DPLEntity> agentResolution;
	map<DPLEntity, DPLModule*> logicFactory;

	DPLNarrativeLogic *pScheduler;
	vector<DustProdLightCore> cores;
	vector<DustProdLightDialog> dialogs;

	DustProdLightStore store;
	map<string, int> dataGlobal;

	DustProdLightRuntime();
	~DustProdLightRuntime();

public:
	static DustProdLightCore* getCurrentCore();

	static DustProdLightEntity *getRootEntity(DPLEntity entity);

	static void init();
	static void release();

	static DPLNarrativeLogic *createAction(DPLEntity eAction);
	static void releaseAction(DPLEntity eAction, DPLNarrativeLogic *pAction);

	DustProdLightEntity *resolveEntity(DPLEntity e);

	static string getMetaEntityId(DPLEntity primaryType, string name, DPLEntity parent);
	static DPLEntity initMetaEntity(int entity, DPLEntity primaryType, string name, DPLEntity parent =
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
	friend class DustProdLightAgent;
	friend class DustProdLightCore;
};

#endif /* DPL_IMPL_PROC_H_ */
