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
#include <fstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

#include "../base/strconv.h"
#include "dplujson.h"

using namespace std;

enum JsonChar {
	JSON_CHR_BACKSLASH,

	JSON_CHR_SLASH,
	JSON_CHR_QUOTE,
	JSON_CHR_TAB,
	JSON_CHR_LINEFEED,
	JSON_CHR_CARRIAGE_RETURN,
	JSON_CHR_FORMFEED,
	JSON_CHR_BACKSPACE,

	JSON_CTRL_UNICODE_LEAD,
	JSON_CTRL_KEY_SEP,
	JSON_CTRL_ARRAY_SEP,
	JSON_CTRL_OBJECT_BEGIN,
	JSON_CTRL_OBJECT_END,
	JSON_CTRL_ARRAY_BEGIN,
	JSON_CTRL_ARRAY_END,
};

const char JSON_CHAR_VALUE[] = "\\/\"tnrfbu:,{}[]";
const int JSON_CHAR_COUNT = strlen(JSON_CHAR_VALUE);

const char ESC_STR[] = "\\/\"\t\n\r\f\b";
const int ESC_COUNT = strlen(ESC_STR);

const char* ESC_END = ESC_STR + ESC_COUNT;
const char* JSON_ESC_END = JSON_CHAR_VALUE + ESC_COUNT;

enum JsonReadStates {
	JSON_READ_VALUE, JSON_READ_STRING, JSON_READ_ESC, JSON_READ_UCHAR,
//	JSON_READ_TOKEN,
	JSON_READ_OBJECT,
	JSON_READ_ARRAY,
};

DPLUEntityToJSON::DPLUEntityToJSON(ostream& os_, bool escJson_) :
		escJson(escJson_), os(&os_) {
//	writeStr("öű\n\"");
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
			const char * p = find(ESC_STR, ESC_END, ch);
			if (p < ESC_END) {
				char r = JSON_CHAR_VALUE[p - ESC_STR];
				out << JSON_CHAR_VALUE[JSON_CHR_BACKSLASH] << r;
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

DPLUJSONToEntity::DPLUJSONToEntity() {
	pos = -1;
	readState = JSON_READ_VALUE;
	uCharPos = uCharVal = 0;
	eTarget = 0;
}

DPLProcessResponse DPLUJSONToEntity::addCodePoint(char32_t cp) {
	++pos;

	if ( isascii(cp)) {
		throw new DPLErrJson(JSON_ERR_NON_ASCII_CHAR, pos);
	}

	const unsigned char chr = (unsigned char) cp;
	const char * p;

	if (isspace(chr)) {
		switch ((JsonReadStates) readState) {
		case JSON_READ_STRING:
			if (' ' == chr) {
				str += (char) chr;
			}
			break;
		case JSON_READ_VALUE:
			if (0 == token) {
				token = DPL::getToken(str);
				str = "";
			}
			break;
		default:
			// eat whitespace
			break;
		}
	} else {
		switch ((JsonReadStates) readState) {
		case JSON_READ_ESC:
			if (chr == JSON_CHAR_VALUE[JSON_CTRL_UNICODE_LEAD]) {
				readState = JSON_READ_UCHAR;
			} else {
				p = find(JSON_CHAR_VALUE, JSON_ESC_END, chr);
				if (p < JSON_ESC_END) {
					char r = ESC_STR[p - JSON_CHAR_VALUE];
					str += r;
					readState = JSON_READ_STRING;
				} else {
					throw new DPLErrJson(JSON_ERR_INVALID_ESCAPE_CHAR, pos);
				}
			}
			break;
		case JSON_READ_UCHAR:
			DPLUCppUtils::addByBase(uCharVal, 16, chr);
			if (++uCharPos > 3) {
				str += uCharVal;
				uCharPos = uCharVal = 0;
				readState = JSON_READ_STRING;
			}
			break;
		case JSON_READ_STRING:
			if (chr == JSON_CHAR_VALUE[JSON_CHR_BACKSLASH]) {
				readState = JSON_READ_ESC;
			} else if (chr == JSON_CHAR_VALUE[JSON_CHR_QUOTE]) {
				readState = JSON_READ_VALUE;
			} else {
				str += chr;
			}
			break;
		case JSON_READ_VALUE:
			if (chr == JSON_CHAR_VALUE[JSON_CHR_BACKSLASH]) {
				readState = JSON_READ_ESC;
			} else if (chr == JSON_CHAR_VALUE[JSON_CHR_QUOTE]) {
				readState = JSON_READ_VALUE;
			} else {
				str += chr;
			}
			break;
		case JSON_READ_OBJECT:
			break;
		case JSON_READ_ARRAY:
			break;

		}
	}

	return DPL_PROCESS_ACCEPT;
}
