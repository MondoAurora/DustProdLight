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
#include <fstream>

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

class DPLUStringContainer {
public:
	virtual ~DPLUStringContainer() {}
	virtual string getString() = 0;
};


class DPLUCharSource {
public:
	virtual ~DPLUCharSource() {
	}
	;

	virtual bool isAvail() = 0;
	virtual unsigned char getNext() = 0;
};

class DPLUCharTarget {
public:
	virtual ~DPLUCharTarget() {} ;

	virtual DPLProcessResult addChar(const unsigned char chr) = 0;
};

class DPLUCodepointTarget {
public:
	virtual ~DPLUCodepointTarget(){};
	virtual DPLProcessResult addCodePoint(char32_t cp) = 0;
};

class DPLUCppUtils {
public:
	static bool testBit(unsigned int value, unsigned int bit);
//	static unsigned int collectValue(unsigned int target, unsigned int shift, unsigned int mask, unsigned int val);
	static bool toStringBase(char* target, unsigned char base, unsigned int value);
	static unsigned int addByBase(unsigned int &target, unsigned char base, unsigned char value);
	static char32_t getNextCodePoint(DPLUCharSource *chrSrc);
};

class DPLUDump: public DPLUCodepointTarget {
	bool encodeUtf8;

public:
	DPLUDump(bool encodeUtf8_) : encodeUtf8(encodeUtf8_) {};
	DPLUDump() : DPLUDump(false) {};

	virtual DPLProcessResult addCodePoint(char32_t cp);
};

class DPLUStringCollector: public DPLUCharTarget, DPLUStringContainer {
	string str;

public:
	virtual DPLProcessResult addChar(const unsigned char chr) {
		str += chr;
		return DPL_PROCESS_ACCEPT;
	}

	virtual string getString() {
		return str;
	}
};

class DPLUStringTester: public DPLUCharTarget {
	bool ignoreCase;
	string str;
	unsigned int pos;
	unsigned int len;

public:
	DPLUStringTester(string toTest, bool ignoreCase_) : ignoreCase(ignoreCase_), str(toTest), pos(0) {
		len = str.length();
	};

	DPLUStringTester(string toTest) : DPLUStringTester(toTest, false) {};

	virtual DPLProcessResult addChar(const unsigned char chr);
};

class DPLUCodeTable {
	const char* source;
	const char* target;
	const unsigned int len;

public:
	DPLUCodeTable(const char* source_, const char* target_, unsigned int len_) : source(source_), target(target_), len(len_) {
	}
	unsigned char resolve(const char chr, bool reverse, const char notFound = '\0');
};


class DPLUCharMatcher: public DPLUCodepointTarget {
	DPLUCodeTable *table;
	bool reverse;
	char value = 0;

public:
	DPLUCharMatcher(DPLUCodeTable *table_, bool reverse_) : table(table_), reverse(reverse_) {};

	virtual DPLProcessResult addChar(const unsigned char chr);

	char getValue() {
		return value;
	}
};

class DPLUStream: public DPLUCharSource {
private:
	fstream inStream;

public:
	DPLUStream(const char *fName);
	virtual ~DPLUStream();

	virtual bool isAvail();
	virtual unsigned char getNext();

	char32_t getNextCodePoint();
	static void process(const char* fName, DPLUCodepointTarget *target);
	static void process(const char* fName, DPLUCharTarget *target);
};

class DPLUString: public DPLUCharSource, DPLUCodepointTarget {
private:
	string str;
	unsigned int ptr = 0;

	void init(string str) {
		this->str = str;
		ptr = 0;
	}

public:
	DPLUString() {
	}
	;
	DPLUString(string str) {
		init(str);
	}
	;

	virtual ~DPLUString() {
	}
	;

	virtual bool isAvail();
	virtual unsigned char getNext();
	virtual DPLProcessResult addCodePoint(char32_t cp);

	char32_t getNextCodePoint();

	void process(string source, DPLUCodepointTarget *pTarget);
	string getString();
};

#endif /* DPLUTILS_H_ */
