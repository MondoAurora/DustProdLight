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

using namespace std;

class DustProdLightDialog;

class DPLUActionDump: public DPLAction {
public:
	virtual ~DPLUActionDump() {
	}
	virtual DPLProcessResult dplProcess();
};

class DPLUActionStreamReader: public DPLAction {
	ifstream inStream;

public:
	virtual ~DPLUActionStreamReader() {
	}
	virtual DPLProcessResult dplProcess();
};


class ProcActionBase : public DPLAction {

};

class ProcActionSequence : public ProcActionBase {
	unsigned int pos = 0;
	bool inSep = false;
};

class ProcActionSelect : public ProcActionBase {
	unsigned int pos = 0;
};

class ProcActionRepeat : public ProcActionBase {
	unsigned int pos = 0;
	bool inSep = false;
};

class DustProdLightAgent {
	DustProdLightDialog *pDialog;

	stack<DPLEntity*> stack;
	map<DPLEntity, map<DPLEntity, DPLAction*>> actionByOwnerAndCommand;

	DPLProcessResult result;

	void open(const void* pInitData);
	void step();
	void stepUp();
	void finish(bool error);

public:
	DustProdLightAgent(DustProdLightDialog *pDialog_);
	virtual ~DustProdLightAgent();

};


class DustProdLightDialog {
	static map<DPLEntity, const DPLModule*> logicFactory;

protected:
	DustProdLightDialog() {}
	virtual ~DustProdLightDialog();

public:
	DPLProcessResult executeProcess();

	friend class DPLMain;
	friend class DPLProc;
};




#endif /* DPL_IMPL_PROC_H_ */
