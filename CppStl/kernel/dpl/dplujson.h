/*
 * dplutils.h
 *
 * Utilities based on DustProdLight API
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPLUJSON_H_
#define DPLUJSON_H_

#include <string>
#include <cstdarg>

#include "dplutils.h"

using namespace std;

enum JsonErr {
	JSON_ERR_NON_ASCII_CHAR,
	JSON_ERR_INVALID_ESCAPE_CHAR,
};

class DPLErrJson {
public:
	const JsonErr err;
	const int pos;

	DPLErrJson(JsonErr e, int p) : err(e), pos(p) {}
};


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

class DPLUJsonLangConsts {
public:
	static const char JSON_CHAR_VALUE[];
	static const char ESC_STR[];

	static const DPLUCodeTable CT_JSON;
};

class DPLUEntityToJSON: public DPLVisitor {
	bool escJson;
	ostream* os;

	bool comma = false;
	string prefix = "";
	string indent = "  ";

protected:
	ostream& target() {
		return *os;
	}

	ostream& closeLine(DPLUtilsIndent chgIndent);
	ostream& leadToken(DPLToken token, DPLUtilsIndent chgIndent);
	ostream& leadToken(DPLToken token);

	ostream& writeStr(string str);

public:
	DPLUEntityToJSON(ostream& os_, bool escJson);
	virtual ~DPLUEntityToJSON();

	virtual DPLFilterResponse shouldProcess(DPLEntity entity, DPLToken token);

	virtual void visitEnd(DPLEntity entity, void *pHint);

	virtual void processValBool(DPLEntity entity, DPLToken token, bool val, void *pHint);
	virtual void processValInt(DPLEntity entity, DPLToken token, int val, void *pHint);
	virtual void processValDouble(DPLEntity entity, DPLToken token, double val, void *pHint);
	virtual void processValString(DPLEntity entity, DPLToken token, string val, void *pHint);

	virtual void processRefBegin(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint);
	virtual void processRefEnd(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint);

	virtual void* processBeginEntity(DPLEntity entity, int key, void* pHint);
	virtual void* processEndEntity(DPLEntity entity, int key, void* pHint);
};

//class DPLUJSONToEntity : public DPLUCodepointTarget {
//private:
//	int pos = 0;
//
//	DPLEntity eTarget;
//	DPLToken token;
//	string str;
//
//	unsigned int uCharVal;
//	char uCharPos;
//
//	int readContext;
//	int readState;
//
//public:
//	DPLUJSONToEntity();
//	virtual DPLProcessResult addCodePoint(char32_t cp);
//};

#endif /* DPLUJSON_H_ */
