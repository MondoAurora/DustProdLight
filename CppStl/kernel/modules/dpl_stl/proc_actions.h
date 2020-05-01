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

class DPLUActionDump: public DPLNativeLogic {
public:
	virtual ~DPLUActionDump() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class DPLUActionStreamReader: public DPLNativeLogic {
	ifstream inStream;
	int pos;

public:
	virtual ~DPLUActionStreamReader() {
	}
	virtual DPLProcessResult dplActionExecute();
	virtual void dplRelease();
};





class ProcActionControl: public DPLNativeLogic {
private:
	map<DPLEntity, DustProdLightState*> mapChildBlocks;
	bool firstCall = true;

protected:
	unsigned int pos = 0;
	bool inSep = false;

	void requestRelay(DPLEntity relay);
	DPLProcessResult optGetChildResult(DPLProcessResult defRet = DPL_PROCESS_ACCEPT_PASS);
	DPLProcessResult optRelayChild();

	virtual bool isColl() {
		return true;
	}

public:
	virtual ~ProcActionControl();

	virtual DPLProcessResult dplResourceInit() {
		firstCall = true;
		return DPLNativeLogic::dplResourceInit();
	}
};

class ProcActionRepeat: public ProcActionControl {
protected:
	virtual bool isColl() {
		return false;
	}

public:
	virtual ~ProcActionRepeat() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class ProcActionSequence: public ProcActionControl {
public:
	virtual ~ProcActionSequence() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class ProcActionSelect: public ProcActionControl {
public:
	virtual ~ProcActionSelect() {
	}
	virtual DPLProcessResult dplActionExecute();
};


#endif /* DPL_IMPL_PROC_ACTIONS_H_ */
