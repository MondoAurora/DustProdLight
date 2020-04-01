/*
 * dplutils.cpp
 *
 * Utilities based on DustProdLight API
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include <string.h>
#include <temp/dplujson.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <map>

//#include "dpl_impl_proc.h"

using namespace std;


const char DPLUJsonLangConsts::JSON_CHAR_VALUE[] = "\\/\"tnrfbu:,{}[]";
const char DPLUJsonLangConsts::ESC_STR[] = "\\/\"\t\n\r\f\b";

const DPLUCodeTable DPLUJsonLangConsts::CT_JSON(JSON_CHAR_VALUE, ESC_STR, strlen(ESC_STR));

//enum JsonReadStates {
//	JSON_READ_VALUE, JSON_READ_STRING, JSON_READ_ESC, JSON_READ_UCHAR,
////	JSON_READ_TOKEN,
//	JSON_READ_OBJECT,
//	JSON_READ_ARRAY,
//};

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

ostream& DPLUEntityToJSON::leadToken(DPLEntity token) {
	return leadToken(token, DPL_INDENT_KEEP);
}

ostream& DPLUEntityToJSON::leadToken(DPLEntity token, DPLUtilsIndent chgIndent) {
	closeLine(chgIndent);
	return writeStr(DPLUtils::getGlobalId(token)) << " : ";
}

ostream& DPLUEntityToJSON::writeStr(string str) {
	ostream& out = target();

	out << "\"";

	DPLUString us(str);

	for (char32_t ch = us.getNextCodePoint(); ch; ch = us.getNextCodePoint()) {
		if (ch < 127) {
			const char escRes = DPLUJsonLangConsts::CT_JSON.resolve(ch, true);
			if (escRes) {
				out << DPLUJsonLangConsts::JSON_CHAR_VALUE[JSON_CHR_BACKSLASH] << escRes;
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

DPLFilterResponse DPLUEntityToJSON::shouldProcess(DPLEntity entity, DPLEntity token) {
	return DPL_FILTER_VISIT;
}

void DPLUEntityToJSON::processValBool(DPLEntity entity, DPLEntity token, bool val, void *pHint) {
	leadToken(token) << (val ? "true" : "false");
}

void DPLUEntityToJSON::processValInt(DPLEntity entity, DPLEntity token, int val, void *pHint) {
	leadToken(token) << val;
}

void DPLUEntityToJSON::processValDouble(DPLEntity entity, DPLEntity token, double val, void *pHint) {
	leadToken(token) << val;
}

void DPLUEntityToJSON::processValString(DPLEntity entity, DPLEntity token, string val, void *pHint) {
	leadToken(token);
	writeStr(val);
}

void DPLUEntityToJSON::processRefBegin(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void *pHint) {
	leadToken(token, DPL_INDENT_INC) << "[";
	comma = false;
}

void DPLUEntityToJSON::processRefEnd(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void *pHint) {
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

enum JsonReaderType {
	JSON_READ_SPACE,
	JSON_READ_UNICODE,
	JSON_READ_ESC,
	JSON_READ_STRING,
	JSON_READ_PLAIN,
	JSON_READ_OBJECT,
	JSON_READ_ARRAY,
	JSON_READ_VALUE,

	JSON_READ_UNSET = 1000,
};

class JSONNodeProcBase;

class JSONProcNodeUnicode;
class JSONProcNodeEsc;
class JSONProcNodePlain;
class JSONProcNodeString;
class JSONProcNodeString;

class JSONNodeProcContext {
public:
	int pos;
	char32_t chr;
	string str;

	JsonReaderType nextRelay = JSON_READ_UNSET;

	map<JsonReaderType, JSONNodeProcBase*> staticRelay;

	JSONNodeProcBase* getNextRelay() {
		if (JSON_READ_UNSET == nextRelay) {
			return NULL;
		} else {
			JSONNodeProcBase* pNext = staticRelay[nextRelay];
			nextRelay = JSON_READ_UNSET;
			return pNext;
			return NULL;
		}
	}

	void charAppend(char32_t c) {
		chr = c;
		str += chr;
	}
};
/*
class JSONNodeProcBase: public DPLUCharTarget, public DPLUProcessorRelayNode {
protected:
	JSONNodeProcContext *pCtx;

	void setRelay(JsonReaderType relay, DPLProcessResult res = DPL_PROCESS_RELAY) {
		pCtx->nextRelay = relay;
		result = res;
	}

public:
	virtual ~JSONNodeProcBase();

	virtual bool isDynamic() {
		return false;
	}
	virtual int getRelayType() = 0;

	virtual DPLUProcessorRelayNode* getRelayChild() {
		return pCtx->getNextRelay();
	}

	virtual DPLProcessResult processRelayChildCompletion(DPLUProcessorRelayNode* pChild, DPLProcessResult childResult) {
		return childResult;
	}
};

class JSONProcNodeSpace: public JSONNodeProcBase {
	virtual int getRelayType() {
		return JSON_READ_SPACE;
	}

	virtual DPLProcessResult addChar(const char chr) {
		result = isspace(chr) ? DPL_PROCESS_ACCEPT : DPL_PROCESS_SUCCESS_RECALL;

		return result;
	}
};

class JSONProcNodeUnicode: public JSONNodeProcBase {
private:
	int pos;
	unsigned int collVal;

public:
	void init() {
		pos = 0;
		collVal = 0;
	}

	virtual int getRelayType() {
		return JSON_READ_UNICODE;
	}

	virtual DPLProcessResult addChar(const char chr) {

		unsigned int digitVal;
		if (DPLUCppUtils::fromStringBase(digitVal, &chr, 16, 1)) {
			collVal <<= 4;
			collVal += digitVal;

			if (++pos < 4) {
				result = DPL_PROCESS_ACCEPT;
			} else {
				pCtx->charAppend(DPLUCppUtils::toUtf8Char(collVal));
				result = DPL_PROCESS_SUCCESS;
			}
		} else {
			result = DPL_PROCESS_REJECT;
		}

		return result;
	}
};

class JSONProcNodeEsc: public JSONNodeProcBase {
	virtual int getRelayType() {
		return JSON_READ_ESC;
	}

	virtual DPLProcessResult addChar(const char chr) {
		if (JSON_CHAR_VALUE[JSON_CTRL_UNICODE_LEAD] == chr) {
			result = DPL_PROCESS_RELAY;
		} else {
			const char chRes = CT_JSON.resolve(chr, false);
			if (chRes) {
				pCtx->charAppend(chRes);
				result = DPL_PROCESS_SUCCESS;
			} else {
				result = DPL_PROCESS_REJECT;
			}
		}

		return result;
	}
};

class JSONProcNodePlain: public JSONNodeProcBase {
public:
	virtual int getRelayType() {
		return JSON_READ_PLAIN;
	}

	virtual DPLProcessResult addChar(const unsigned char chr) {
		if (isalnum(chr)) {
			pCtx->charAppend(chr);
		} else if (isspace(chr)) {
			result = DPL_PROCESS_SUCCESS;
		} else if ((JSON_CHAR_VALUE[JSON_CTRL_ARRAY_SEP] == chr) || (JSON_CHAR_VALUE[JSON_CTRL_ARRAY_END] == chr)
				|| (JSON_CHAR_VALUE[JSON_CTRL_OBJECT_END] == chr)) {
			result = DPL_PROCESS_SUCCESS_RECALL;
		} else {
			result = DPL_PROCESS_REJECT;
		}

		return result;
	}
};

class JSONProcNodeString: public JSONNodeProcBase {
public:
	virtual int getRelayType() {
		return JSON_READ_STRING;
	}

	virtual DPLProcessResult processRelayChildCompletion(DPLUProcessorRelayNode* pChild, DPLProcessResult childResult) {
		if (DPL_PROCESS_SUCCESS == childResult) {
			result = DPL_PROCESS_ACCEPT;
		} else {
			result = childResult;
		}

		return result;
	}

	virtual DPLProcessResult addChar(const unsigned char chr) {
		if (JSON_CHAR_VALUE[JSON_CHR_BACKSLASH] == chr) {
			setRelay(JSON_READ_ESC);
		} else if (JSON_CHAR_VALUE[JSON_CHR_QUOTE] == chr) {
			result = DPL_PROCESS_SUCCESS;
		} else {
			pCtx->charAppend(chr);
		}

		return result;
	}
};

class JSONProcNodeArray: public JSONNodeProcBase {
public:
	virtual int getRelayType() {
		return JSON_READ_ARRAY;
	}

	virtual DPLProcessResult processRelayChildCompletion(DPLUProcessorRelayNode* pChild, DPLProcessResult childResult) {
		if (DPL_PROCESS_SUCCESS == childResult) {
			if ( JSON_READ_SPACE == pChild->getRelayType()) {
				setRelay(JSON_READ_VALUE, childResult);
			}
			result = DPL_PROCESS_ACCEPT;
		} else {
			result = childResult;
		}

		return result;
	}

	virtual DPLProcessResult addChar(const unsigned char chr) {
		if ( isspace(chr )) {
			setRelay(JSON_READ_SPACE);
		} else {
			setRelay(JSON_READ_VALUE, DPL_PROCESS_RELAY_RECALL);
		}
		return result;
	}
};

*/

//DPLUJSONToEntity::DPLUJSONToEntity() {
//	pos = -1;
//	readState = readContext = JSON_READ_VALUE;
//	uCharPos = uCharVal = 0;
//	eTarget = token = 0;
//}

//DPLProcessResult DPLUJSONToEntity::addCodePoint(char32_t cp) {
//	++pos;
//
//	if ( isascii(cp)) {
//		throw new DPLErrJson(JSON_ERR_NON_ASCII_CHAR, pos);
//	}
//
//	const unsigned char chr = (unsigned char) cp;
////	const char * p;
//
//	if (isspace(chr)) {
//		switch ((JsonReadStates) readState) {
//		case JSON_READ_STRING:
//			if (' ' == chr) {
//				str += (char) chr;
//			}
//			break;
//		case JSON_READ_VALUE:
//			if (0 == token) {
//				token = DPL::getToken(str);
//				str = "";
//			}
//			break;
//		default:
//			// eat whitespace
//			break;
//		}
//	} else {
//		switch ((JsonReadStates) readState) {
//		case JSON_READ_ESC:
//			if (chr == JSON_CHAR_VALUE[JSON_CTRL_UNICODE_LEAD]) {
//				readState = JSON_READ_UCHAR;
//			} else {
////				p = find(JSON_CHAR_VALUE, JSON_ESC_END, chr);
////				if (p < JSON_ESC_END) {
////					char r = ESC_STR[p - JSON_CHAR_VALUE];
////					str += r;
////					readState = JSON_READ_STRING;
////				} else {
////					throw new DPLErrJson(JSON_ERR_INVALID_ESCAPE_CHAR, pos);
////				}
//			}
//			break;
//		case JSON_READ_UCHAR:
//			DPLUCppUtils::addByBase(uCharVal, 16, chr);
//			if (++uCharPos > 3) {
//				str += uCharVal;
//				uCharPos = uCharVal = 0;
//				readState = JSON_READ_STRING;
//			}
//			break;
//		case JSON_READ_STRING:
//			if (chr == JSON_CHAR_VALUE[JSON_CHR_BACKSLASH]) {
//				readState = JSON_READ_ESC;
//			} else if (chr == JSON_CHAR_VALUE[JSON_CHR_QUOTE]) {
//				readState = JSON_READ_VALUE;
//			} else {
//				str += chr;
//			}
//			break;
//		case JSON_READ_VALUE:
//			if (chr == JSON_CHAR_VALUE[JSON_CHR_BACKSLASH]) {
//				readState = JSON_READ_ESC;
//			} else if (chr == JSON_CHAR_VALUE[JSON_CHR_QUOTE]) {
//				readState = JSON_READ_VALUE;
//			} else {
//				str += chr;
//			}
//			break;
//		case JSON_READ_OBJECT:
//			break;
//		case JSON_READ_ARRAY:
//			break;
//
//		}
//	}
//
//	return DPL_PROCESS_ACCEPT;
//}
