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

#include "../dpl_impl.h"

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
	ostream& leadToken(DPLEntity token, DPLUtilsIndent chgIndent);
	ostream& leadToken(DPLEntity token);

	ostream& writeStr(string str);

public:
	DPLUEntityToJSON(ostream& os_, bool escJson);
	virtual ~DPLUEntityToJSON();

	virtual DPLFilterResponse shouldProcess(DPLEntity entity, DPLEntity token);

	virtual void visitEnd(DPLEntity entity, void *pHint);

	virtual void processValBool(DPLEntity entity, DPLEntity token, bool val, void *pHint);
	virtual void processValInt(DPLEntity entity, DPLEntity token, int val, void *pHint);
	virtual void processValDouble(DPLEntity entity, DPLEntity token, double val, void *pHint);
	virtual void processValString(DPLEntity entity, DPLEntity token, string val, void *pHint);

	virtual void processRefBegin(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void *pHint);
	virtual void processRefEnd(DPLEntity entity, DPLEntity token, DPLTokenType tokenType, void *pHint);

	virtual void* processBeginEntity(DPLEntity entity, int key, void* pHint);
	virtual void* processEndEntity(DPLEntity entity, int key, void* pHint);
};

namespace DPLJson {
	void shutdown();
	void read(const char* fileName);
}

#endif /* DPLUJSON_H_ */
