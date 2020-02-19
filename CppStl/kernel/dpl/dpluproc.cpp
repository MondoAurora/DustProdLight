/*
 * dpluproc.cpp
 *
 * Process implementations
 *
 *  Created on: Feb 18, 2020
 *      Author: Lorand Kedves
 */

#include "dpluproc.h"

using namespace std;

//	stack<DPLProcessorRelayNode*> stack;
//	DPLProcessorRelayNode* current;

DPLUProcessorRelayStack::~DPLUProcessorRelayStack() {
	finish();
}

void DPLUProcessorRelayStack::init(DPLUProcessorRelayNode* root) {
	stack.push(root);
	result = DPL_PROCESS_ACCEPT;
}

void DPLUProcessorRelayStack::stepUp() {
	DPLUProcessorRelayNode* pn = stack.top();
	stack.pop();
	DPLProcessResult cr = stack.top()->processRelayChildCompletion(pn, result);
	if ( pn->isDynamic() ) {
		delete pn;
	}
	if (DPL_PROCESS_REJECT != result) {
		result = cr;
	}
}

//, , ,
//DPL_PROCESS_RELAY, DPL_PROCESS_RELAY_RECALL, ,

DPLProcessResult DPLUProcessorRelayStack::step() {
	result = stack.top()->getProcessResult();

	switch ( result ) {
	case DPL_PROCESS_ACCEPT:
		// fine, do nothing
		break;
	case DPL_PROCESS_REJECT:
		finish();
		break;
	case DPL_PROCESS_RELAY:
	case DPL_PROCESS_RELAY_RECALL:
		stack.push(stack.top()->getRelayChild());
		result = DPL_PROCESS_ACCEPT;
		break;
	case DPL_PROCESS_SUCCESS:
	case DPL_PROCESS_SUCCESS_RECALL:
		do {
			stepUp();
		} while ((DPL_PROCESS_SUCCESS == result) || (DPL_PROCESS_SUCCESS_RECALL == result));
		if ( DPL_PROCESS_REJECT == result ) {
			finish();
		}
		break;
	}

	return getProcessResult();
}

void DPLUProcessorRelayStack::finish() {
	while (!stack.empty()) {
		stepUp();
	}
}
