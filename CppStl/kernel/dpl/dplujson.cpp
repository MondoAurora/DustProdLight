/*
 * dplutils.cpp
 *
 * Utilities based on DustProdLight API
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include <string.h>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>

#include "../base/strconv.h"
#include "dplujson.h"

using namespace std;
char ESC_JSON_FLAG = '\\';
char ESC_JSON_UNICODE = 'u';
char ESC_STR[] = "\"\\/\b\f\r\n\t";
int ESC_COUNT = strlen(ESC_STR);
char ESC_JSON[] = "\"\\/bfrnt";

DPLUEntityToJSON::DPLUEntityToJSON(ostream& os_, bool escJson_) :
		escJson(escJson_), os(&os_) {
	writeStr("öű\n\"");
}

DPLUEntityToJSON::~DPLUEntityToJSON() {
}

ostream& DPLUEntityToJSON::closeLine(DPLUtilsIndent chgIndent) {
	ostream& out = target();

	if (comma) {
		out << ",";
	} else {
		comma = true;
	}

	out << endl;

	switch (chgIndent) {
	case DPL_INDENT_KEEP:
		out << prefix;
		break;
	case DPL_INDENT_INC:
		out << prefix;
		prefix = prefix + indent;
		break;
	case DPL_INDENT_DEC:
		prefix = prefix.substr(0, prefix.length() - indent.length());
		out << prefix;
		break;
	}

	return out;
}

ostream& DPLUEntityToJSON::leadToken(DPLToken token) {
	return leadToken(token, DPL_INDENT_KEEP);
}

ostream& DPLUEntityToJSON::leadToken(DPLToken token, DPLUtilsIndent chgIndent) {
	closeLine(chgIndent);
	return writeStr(DPLUtils::getTokenName(token)) << " : ";
}

ostream& DPLUEntityToJSON::writeStr(string str) {
	ostream& out = target();

	out << "\"";

	DPLUString us(str);

	for (char32_t ch = us.getNextCodePoint(); ch; ch = us.getNextCodePoint()) {
		if (ch < 127) {
			// ASCII
			char * end = ESC_STR + ESC_COUNT;
			char * p = find(ESC_STR, end, ch);
			if (p < end) {
				char r = ESC_JSON[p - ESC_STR];
				out << ESC_JSON_FLAG << r;
			} else {
				out << (char) ch;
			}
		} else {
			char encoded[] = "\\u0000";
			DPLUCppUtils::toStringBase(encoded, 16, ch);
			out << encoded;
		}
	}

	return out << "\"";
}

DPLFilterResponse DPLUEntityToJSON::shouldProcess(DPLEntity entity, DPLToken token) {
	return DPL_FILTER_VISIT;
}

void DPLUEntityToJSON::processValBool(DPLEntity entity, DPLToken token, bool val, void *pHint) {
	leadToken(token) << (val ? "true" : "false");
}

void DPLUEntityToJSON::processValInt(DPLEntity entity, DPLToken token, int val, void *pHint) {
	leadToken(token) << val;
}

void DPLUEntityToJSON::processValDouble(DPLEntity entity, DPLToken token, double val, void *pHint) {
	leadToken(token) << val;
}

void DPLUEntityToJSON::processValString(DPLEntity entity, DPLToken token, string val, void *pHint) {
	leadToken(token);
	writeStr(val);
}

void DPLUEntityToJSON::processRefBegin(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {
	leadToken(token, DPL_INDENT_INC) << "[";
	comma = false;
}

void DPLUEntityToJSON::processRefEnd(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint) {
	comma = false;
	closeLine(DPL_INDENT_DEC) << "]";
}

void* DPLUEntityToJSON::processBeginEntity(DPLEntity entity, int key, void* pHint) {
	closeLine(DPL_INDENT_INC) << "{";
	comma = false;
	return pHint;
}

void* DPLUEntityToJSON::processEndEntity(DPLEntity entity, int key, void* pHint) {
	comma = false;
	closeLine(DPL_INDENT_DEC) << "}";
	return pHint;
}

void DPLUEntityToJSON::visitEnd(DPLEntity entity, void *pHint) {
	target() << endl << endl;
}
