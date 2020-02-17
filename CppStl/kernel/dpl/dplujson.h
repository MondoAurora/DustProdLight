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

class DPLUJSONToEntity : public DPLUCodepointTarget {
private:
	int pos = 0;

	DPLEntity eTarget;
	DPLToken token;
	string str;

	unsigned int uCharVal;
	char uCharPos;

	int readContext;
	int readState;

public:
	DPLUJSONToEntity();
	virtual DPLProcessResponse addCodePoint(char32_t cp);
};

#endif /* DPLUJSON_H_ */
