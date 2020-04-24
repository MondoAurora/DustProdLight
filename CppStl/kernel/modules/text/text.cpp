#include <iostream>

#include "text.h"

using namespace std;

using namespace DPLUnitNarrative;
using namespace DPLUnitTools;

DPLProcessResult CharMatcher::dplActionExecute() {
	return DPL_PROCESS_ACCEPT;
}

DPLProcessResult TokenMatcher::dplActionExecute() {
	return DPL_PROCESS_ACCEPT;
}

DPLProcessResult NumberSci::dplActionExecute() {
	return DPL_PROCESS_ACCEPT;
}

DPLProcessResult NumberRad::dplActionExecute() {
	return DPL_PROCESS_ACCEPT;
}
