/*
 * dpl_impl_proc.h
 *
 * Process services
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_PROC_ACTIONS_H_
#define DPL_IMPL_PROC_ACTIONS_H_

#include <stack>
#include <map>
#include <set>
#include <vector>
#include <array>
#include <fstream>

#include "proc_runtime.h"

using namespace std;

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





class ProcActionControl: public DPLAction {
private:
	map<DPLEntity, DustProdLightBlock*> mapChildBlocks;

protected:
	void requestRelay(DPLEntity relay);

public:
	virtual ~ProcActionControl();
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






class DustProdLightDialogTokenRing: public DPLAction {
	vector<DustProdLightAgent*> agents;
	int currAgent = 0;

public:
	DustProdLightDialogTokenRing();
	virtual ~DustProdLightDialogTokenRing();

	virtual DPLProcessResult dplProcess();

	friend class DPLMain;
	friend class DPLProc;
};

#endif /* DPL_IMPL_PROC_ACTIONS_H_ */
