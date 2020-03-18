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

#include "../dpl_impl_meta.h"
#include "../dpl_impl.h"

using namespace std;

class DustProdLightDialog;
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


class ProcActionSignal : public DPLAction {
public:
	virtual ~ProcActionSignal() {
	}
	virtual DPLProcessResult dplProcess();
};


class ProcActionControl : public DPLAction {
public:
	virtual ~ProcActionControl() {
	}
};

class ProcActionSequence : public ProcActionControl {
	unsigned int pos = 0;
	bool inSep = false;

public:
	virtual ~ProcActionSequence() {
	}
	virtual DPLProcessResult dplProcess();
	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse);
};

class ProcActionSelect : public ProcActionControl {
	unsigned int pos = 0;

public:
	virtual ~ProcActionSelect() {
	}
	virtual DPLProcessResult dplProcess();
	virtual DPLProcessResult dplChildReturned(DPLProcessResult childResponse);
};

class ProcActionRepeat : public ProcActionControl {
	unsigned int pos = 0;
	bool inSep = false;
	int min = 0;
	int max = INT_MAX;
	int count = 0;

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

public:
	DustProdLightProcess(DustProdLightProcess *pParent_) : pParent(pParent_){}
	~DustProdLightProcess(){}

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightImplementation;
};

class DustProdLightThread {
	DustProdLightProcess *pProcess;

	DustProdLightAgent* pAgent = NULL;
	volatile bool requestSuspend = false;
	DPLProcessResult result = DPL_PROCESS_ACCEPT;

public:
	DustProdLightThread(DustProdLightProcess *pProcess_) : pProcess(pProcess_) {}
	~DustProdLightThread(){};
};

class DustProdLightBlock {
private:
	DPLBlock blockType;
	map<int, DustProdLightEntity*> emapRef;
	map<int, DustProdLightEntity> emapLocal;

	DPLAction* pAction;

public:
	DustProdLightEntity* getEntity(DPLEntity e) {
		EntityIterator i = emapLocal.find(e);
		DustProdLightEntity* ret;

		if ( i == emapLocal.end() ) {
			EntityPtrIterator pi = emapRef.find(e);
			ret = ( pi == emapRef.end()) ? NULL : pi->second;
		} else {
			ret = &i->second;
		}

		return ret;
	}
};

class DustProdLightAgent : public DPLAction {
	DustProdLightDialog *pDialog;

	stack<DustProdLightBlock*> stack;

	DPLProcessResult result;

	void init(DPLEntity eAgentDef, DustProdLightDialog *pDialog_);
	void step();
	void stepUp();
	void finish(bool error);

public:
	DustProdLightAgent();
	virtual ~DustProdLightAgent();

	virtual DPLProcessResult dplProcess();
};


class DustProdLightDialog : public DPLAction {
	map<DPLEntity, DustProdLightAgent*> agents;
	DustProdLightAgent* pActiveAgent;

public:
	DustProdLightDialog();
	virtual ~DustProdLightDialog();

	virtual DPLProcessResult dplProcess();

	friend class DPLMain;
	friend class DPLProc;
};


class DustProdLightRuntime {
	static map<DPLEntity, const DPLModule*> logicFactory;

	static DustProdLightProcess *pProcessMain;
	static DustProdLightThread *pThreadActive;

	static int nextEntityId;
	static map<string, int> dataGlobal;

public:
	static void init();
	static void release();

	static DustProdLightEntity *resolveEntity(DPLEntity e);

	friend class DPLData;
	friend class DPLMain;
	friend class DustProdLightImplementation;
};



#endif /* DPL_IMPL_PROC_H_ */
