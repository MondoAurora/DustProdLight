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

class DustProdLightPDA;

class DustProdLightState: public DPLNativeLogic {
private:
	map<int, DustProdLightEntity*> emapRef;
	DustProdLightStore *pStore;

	DPLNativeLogic* pLogic;

public:
	DustProdLightState();
	~DustProdLightState();

	DustProdLightEntity* getEntity(DPLEntity e) ;

	DPLProcessResult init(DustProdLightEntity *pSelf, DustProdLightState *pParent);

	virtual DPLProcessResult dplActionExecute();
	virtual DPLProcessResult dplResourceRelease();

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
	friend class DustProdLightPDA;
	friend class ProcActionControl;
};

typedef map<DPLEntity, DustProdLightState*>::const_iterator StateIterator;

class DustProdLightPDA: public DPLNativeLogic {
	map<int, DustProdLightState*> stack;
	int stackPos = -1;

public:
	DustProdLightPDA();
	virtual ~DustProdLightPDA();

	virtual DPLProcessResult dplActionExecute();
	virtual DPLProcessResult dplResourceRelease();

	DustProdLightState* getCurrentState() {
		return stack[stackPos];
	}

	void relayEntry(DustProdLightState *pStateRelay);
	void relayExit();

	DustProdLightEntity* resolveEntity(DPLEntity entity) {
		DustProdLightState* pS = stack[stackPos];
		return pS ? pS->getEntity(entity) : NULL;
	}

	friend class DustProdLightRuntime;
	friend class ProcActionControl;
	friend class DustProdLightDialog;
};

class DustProdLightDialog : public DPLNativeLogic {
	vector<DustProdLightPDA> pdas;
	unsigned int currPDAIdx = 0;
	DustProdLightEntity *pData;

public:
	DustProdLightDialog();
	~DustProdLightDialog();

	DustProdLightPDA* getCurrentPda() {
		return &pdas[currPDAIdx];
	}

	DustProdLightPDA* getPda(unsigned int idx) {
		if ( idx >= pdas.size() ) {
			pdas.resize(idx+1);
		}

		return &pdas[idx];
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

class DustProdLightCore : public DPLNativeLogic {
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

	DPLProcessResult run(int dlgIdx, DustProdLightState *pState, int agentCount = 1);

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightState;
	friend class DustProdLightPDA;
	friend class DustProdLightRuntime;
	friend class ProcActionControl;
};

class DustProdLightRuntime {
	static DustProdLightRuntime *pRuntime;

	map<DPLEntity, DPLEntity> agentResolution;
	map<DPLEntity, DPLModule*> logicFactory;

	DPLNativeLogic *pScheduler;
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

	static DPLNativeLogic *createLogic(DPLEntity eAction);
	static void releaseLogic(DPLEntity eAction, DPLNativeLogic *pAction);

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
	friend class DustProdLightState;
	friend class DustProdLightPDA;
	friend class DustProdLightCore;
};

#endif /* DPL_IMPL_PROC_H_ */
