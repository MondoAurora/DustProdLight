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

class DPLUActionDump: public DPLNarrativeLogic {
public:
	virtual ~DPLUActionDump() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class DPLUActionStreamReader: public DPLNarrativeLogic {
	ifstream inStream;
	int pos;

public:
	virtual ~DPLUActionStreamReader() {
	}
	virtual DPLProcessResult dplActionExecute();
	virtual void dplRelease();
};





class ProcActionControl: public DPLNarrativeLogic {
private:
	map<DPLEntity, DustProdLightBlock*> mapChildBlocks;
	bool firstCall = true;

protected:
	void requestRelay(DPLEntity relay);
	DPLProcessResult optGetChildResult(DPLProcessResult defRet = DPL_PROCESS_SUCCESS);

public:
	virtual ~ProcActionControl();

	virtual DPLProcessResult dplResourceInit() {
		firstCall = true;
		return DPLNarrativeLogic::dplResourceInit();
	}
};

class ProcActionRepeat: public ProcActionControl {
	unsigned int count = 0;
	bool inSep = false;
	unsigned int min = 0;
	unsigned int max = INT_MAX;

public:
	virtual ~ProcActionRepeat() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class ProcActionSequence: public ProcActionControl {
	unsigned int pos = 0;
	bool inSep = false;

public:
	virtual ~ProcActionSequence() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class ProcActionSelect: public ProcActionControl {
	unsigned int pos = 0;

public:
	virtual ~ProcActionSelect() {
	}
	virtual DPLProcessResult dplActionExecute();
};


#endif /* DPL_IMPL_PROC_ACTIONS_H_ */
