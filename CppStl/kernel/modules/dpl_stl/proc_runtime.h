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

class DustProdLightBlock: public DPLAction {
private:
	DPLBlock blockType;
	DPLEntity cmd;

	map<int, DustProdLightEntity*> emapRef;
	DustProdLightStore *pStore;

	DPLAction* pAction;
	DustProdLightAgent *pOwnAgent = NULL;

public:
	DustProdLightBlock();
	~DustProdLightBlock();

	DustProdLightEntity* getEntity(DPLEntity e) ;

	DPLProcessResult init(DustProdLightEntity *pSelf, DustProdLightBlock *pParent);

	virtual DPLProcessResult dplProcess();
	void release();

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
	friend class DustProdLightAgent;
	friend class ProcActionControl;
};

typedef map<DPLEntity, DustProdLightBlock*>::const_iterator BlockIterator;

class DustProdLightAgent: public DPLAction {
	map<int, DustProdLightBlock*> stack;
	int stackPos = 0;

public:
	DustProdLightAgent();
	virtual ~DustProdLightAgent();

	virtual DPLProcessResult dplProcess();

	friend class DustProdLightRuntime;
	friend class ProcActionControl;
};

class DustProdLightDialog : public DPLAction {
public:
	~DustProdLightDialog() {
	}

	virtual DPLProcessResult dplProcess();

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightRuntime;
};

class DustProdLightSheduler : public DPLAction {
};

class DustProdLightCore : public DPLAction {
	DustProdLightDialog *pDialog = NULL;
	DustProdLightAgent *pAgent = NULL;
	DustProdLightBlock *pBlock = NULL;

	DPLProcessResult lastResult;

public:
	static DustProdLightCore* getCurrentCore();

	virtual DPLProcessResult dplProcess();

	DPLProcessResult getLastResult () {
		return lastResult;
	}

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightBlock;
	friend class DustProdLightAgent;
	friend class DustProdLightRuntime;
	friend class ProcActionControl;
};

class DustProdLightCoreSingle :public DustProdLightCore {
	static DustProdLightCoreSingle singleCore;

	friend class DustProdLightCore;
};

class DustProdLightRuntime {
	static DustProdLightRuntime *pRuntime;

	DustProdLightSheduler *pScheduler = NULL;

	map<DPLEntity, DPLModule*> logicFactory;

	map<string, int> dataGlobal;
	DustProdLightStore store;

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
	friend class DustProdLightAgent;
	friend class DustProdLightCore;
};

#endif /* DPL_IMPL_PROC_H_ */
