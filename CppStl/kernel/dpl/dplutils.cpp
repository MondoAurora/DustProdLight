/*
 * dplutils.cpp
 *
 * Utilities based on DustProdLight API
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include <iostream>

#include "dplutils.h"
#include "../base/strconv.h"

using namespace std;

DPLEntityDumper::~DPLEntityDumper() {
}

ostream& DPLEntityDumper::closeLine(DPLUtilsIndent chgIndent) {
	if ( comma ) {
		cout << ",";
	} else {
		comma = true;
	}

	cout << endl;

	switch (chgIndent) {
	case DPL_INDENT_KEEP:
		cout << prefix;
		break;
	case DPL_INDENT_INC:
		cout << prefix;
		prefix = prefix + indent;
		break;
	case DPL_INDENT_DEC:
		prefix = prefix.substr(0, prefix.length() - indent.length());
		cout << prefix;
		break;
	}

	return cout;
}

ostream& DPLEntityDumper::leadToken(DPLToken token) {
	return leadToken(token, DPL_INDENT_KEEP);
}

ostream& DPLEntityDumper::leadToken(DPLToken token, DPLUtilsIndent chgIndent) {
	closeLine(chgIndent) << "\"" << DPLUtils::getTokenName(token) << "\" : ";
	return cout;
}

DPLFilterResponse DPLEntityDumper::shouldProcess(DPLEntity entity, DPLToken token) {
	return DPL_FILTER_VISIT;
}

void DPLEntityDumper::processValBool(DPLEntity entity, DPLToken token, bool val, void *pHint) {
	leadToken(token) << (val ? "true" : "false");
}

void DPLEntityDumper::processValInt(DPLEntity entity, DPLToken token, int val, void *pHint) {
	leadToken(token) << val;
}

void DPLEntityDumper::processValDouble(DPLEntity entity, DPLToken token, double val, void *pHint) {
	leadToken(token) << val;
}

void DPLEntityDumper::processValString(DPLEntity entity, DPLToken token, string val, void *pHint) {
	leadToken(token) << "\"" << val << "\"";
}

void DPLEntityDumper::processRefBegin(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {
	leadToken(token, DPL_INDENT_INC) << "[";
	comma = false;
}

void DPLEntityDumper::processRefEnd(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {
	comma = false;
	closeLine(DPL_INDENT_DEC) << "]";
}

void* DPLEntityDumper::processBeginEntity(DPLEntity entity, int key, void* pHint) {
	closeLine(DPL_INDENT_INC) << "{";
	comma = false;
	return pHint;
}

void* DPLEntityDumper::processEndEntity(DPLEntity entity, int key, void* pHint) {
	comma = false;
	closeLine(DPL_INDENT_DEC) << "}";
	return pHint;
}

void DPLEntityDumper::visitEnd(DPLEntity entity, void *pHint) {
	cout << endl << endl;
}
