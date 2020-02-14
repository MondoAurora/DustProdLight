/*
 * dplutils.h
 *
 * Utilities based on DustProdLight API
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPLUTILS_H_
#define DPLUTILS_H_

#include <string>
#include <cstdarg>

#include "dpl.h"

using namespace std;

enum DPLUtilsIndent {
	DPL_INDENT_KEEP, DPL_INDENT_INC, DPL_INDENT_DEC,
};


class DPLUtils {
public:
	static string getTypeName(DPLType type);
	static string getTokenName(DPLToken token);
};

class DPLEntityDumper: public DPLVisitor {
	bool comma = false;
	string prefix = "";
	string indent = "  ";

	ostream& closeLine(DPLUtilsIndent chgIndent);
	ostream& leadToken(DPLToken token, DPLUtilsIndent chgIndent);
	ostream& leadToken(DPLToken token);

public:
	virtual ~DPLEntityDumper();

	virtual DPLFilterResponse shouldProcess(DPLEntity entity, DPLToken token);

	virtual void visitEnd(DPLEntity entity, void *pHint);

	virtual void processValInt(DPLEntity entity, DPLToken token, int val, void *pHint);
	virtual void processValDouble(DPLEntity entity, DPLToken token, double val, void *pHint);
	virtual void processValString(DPLEntity entity, DPLToken token, string val, void *pHint);

	virtual void processRefBegin(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint);
	virtual void processRefEnd(DPLEntity entity, DPLToken token, DPLTokenType tokenType, void *pHint);

	virtual void* processBeginEntity(DPLEntity entity, int key, void* pHint);
	virtual void* processEndEntity(DPLEntity entity, int key, void* pHint);
};

#endif /* DPLUTILS_H_ */
