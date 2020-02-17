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

class DPLUCharSource {
public:
	virtual ~DPLUCharSource(){};

	virtual unsigned char getNext() = 0;
};

class DPLUCodepointTarget {
public:
	virtual ~DPLUCodepointTarget(){};
	virtual void addCodePoint(char32_t cp) = 0;
};

//class DPLUCodepointReader {
//private:
//	DPLUCharSource *chrSrc;
//
//	unsigned int step = 0;
//	unsigned char head = 0;
//	char32_t codePoint = 0;
//	char32_t headMask = 0;
//
//	bool optAppend();
//
//public:
//	DPLUCodepointReader() :chrSrc(NULL) {	}
//	DPLUCodepointReader(DPLUCharSource *chrSrc_) :chrSrc(chrSrc_) {	}
//	void init(DPLUCharSource *chrSrc_) {
//		chrSrc = chrSrc_;
//	}
//	char32_t getNextCodePoint();
//};

class DPLUCppUtils {
public:
	static bool testBit(unsigned int value, unsigned int bit);
	static unsigned int collectValue(unsigned int target, unsigned int shift, unsigned int mask, unsigned int val);
	static bool toStringBase(char* target, unsigned char base, unsigned int value);
	static char32_t getCodePoint(DPLUCharSource *chrSrc);
};


class DPLUString: public DPLUCharSource, DPLUCodepointTarget {
private:
	string str;
	unsigned int ptr = 0;
//	DPLUCodepointReader cpReader;

	void init(string str) {
		this->str = str;
		ptr = 0;
	}

public:
	DPLUString(){};
	DPLUString(string str){
		init(str);
//		cpReader.init(this);
	};

	virtual ~DPLUString(){};

	virtual unsigned char getNext();
	virtual void addCodePoint(char32_t cp);

	char32_t getNextCodePoint();

	void process(string source, DPLUCodepointTarget *pTarget);
	string getString();
};

#endif /* DPLUTILS_H_ */
