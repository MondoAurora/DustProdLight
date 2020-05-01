/*
 * dplutils.cpp
 *
 * Utilities based on DustProdLight API
 *
 * DPLUStringUtils::getCodePoint() based on http://www.nubaria.com/en/blog/?p=371
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#include <string.h>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>

#include "dplutils.h"

const char BASE_CHAR_LOW_START = 'a';
const char BASE_CHAR_LOW_END = 'z';
const char BASE_CHAR_UP_START = 'A';
const char BASE_CHAR_UP_END = 'Z';

const int BASE_CHAR = BASE_CHAR_LOW_START - 10;
const int BASE_MAX = BASE_CHAR_LOW_END - BASE_CHAR;

bool DPLUCppUtils::toStringBase(char* target, unsigned char base, unsigned int value, bool fill) {
	if (BASE_MAX < base) {
		return false;
	}

	int idx = strlen(target);
	unsigned int rem = value;

	if (value || fill) {
		for (; idx && (fill || rem); rem = rem / base) {
			int m = rem % base;
			target[--idx] = m + ((m < 10) ? '0' : BASE_CHAR);
		}
	} else {
		target[idx - 1] = '0';
	}

	return (0 == rem);
}

bool DPLUCppUtils::optOffset(unsigned int &target, const char val, const char start, const char end) {
	if ((start <= val) && (val <= end)) {
		target = val - start;
		return true;
	} else {
		return false;
	}
}

bool DPLUCppUtils::fromStringBase(unsigned int &target, const char* source, unsigned char base,
		unsigned int length) {
	if (BASE_MAX < base) {
		return false;
	}

	target = 0;

	int l = (0 == length) ? strlen(source) : length;

	for (int idx = 0; idx < l; ++idx) {
		char c = source[idx];

		target *= base;
		unsigned int diff;

		if (optOffset(diff, c, '0', '9') || optOffset(diff, c, BASE_CHAR_LOW_START, BASE_CHAR_LOW_END)
				|| optOffset(diff, c, BASE_CHAR_UP_START, BASE_CHAR_UP_END)) {
			target = target + c - diff;
		} else {
			return false;
		}
	}

	return true;
}


bool DPLUCppUtils::testBit(unsigned int value, unsigned int bit) {
	unsigned int chk = 1 << bit;
	return value & chk;
}

void DPLUCppUtils::setBit(unsigned int &value, unsigned int bit) {
	unsigned int chk = 1 << bit;
	value &= chk;
}

unsigned char CPCHK_BIT[] = { 7, 5, 4 };
unsigned char CPCHK_MASK[] = { 0x1f, 0x0f, 0x07 };

char32_t DPLUCppUtils::getNextCodePoint(DPLUCharSource *chrSrc) {
	unsigned int step = 0;
	unsigned char head = chrSrc->getNext();
	int headLead = 0;
	char32_t codePoint = 0;

	while ((step < 4) && DPLUCppUtils::testBit(head, CPCHK_BIT[step++])) {
		codePoint = (codePoint << 6) + (chrSrc->getNext() & 0x3f);
		headLead += 6;
	}

	if (0 == headLead) {
		codePoint = head;
	} else {
		codePoint += (head & CPCHK_MASK[step]) << headLead;
	}

	return codePoint;
}

char32_t DPLUCppUtils::toUtf8Char(unsigned int val) {
	char32_t codePoint;

	if (val > 0x7f) {
		unsigned int headFlag = 0;
		int shiftIdx = 0;

		for (unsigned char chVal ; val; ) {
			chVal = val & 0x3f;
			codePoint = (codePoint << 8) + chVal;
			val >>= 6;
			setBit(headFlag, CPCHK_BIT[shiftIdx++]);
		}

		codePoint += headFlag << (8*shiftIdx);

	} else {
		codePoint = val;
	}

	return codePoint;
}

DPLProcessResult DPLUStringTester::addChar(const char chr) {
	if (ignoreCase ? (tolower(chr) == (char) tolower(str[pos])) : str[pos] == chr) {
		return (++pos < len) ? DPL_PROCESS_ACCEPT_READ : DPL_PROCESS_ACCEPT;
	} else {
		pos = 0;
		return DPL_PROCESS_REJECT;
	}
}

unsigned char DPLUCodeTable::resolve(const char chr, bool reverse, const char notFound) const {
	const char * s = reverse ? target : source;
	const char * t = reverse ? source : target;
	const char * e = s + len;

	const char * p = find(s, e, chr);

	return (p < e) ? t[p - s] : notFound;
}

DPLProcessResult DPLUCharMatcher::addChar(const char chr) {
	value = table->resolve(chr, reverse);
	return value ? DPL_PROCESS_ACCEPT : DPL_PROCESS_REJECT;
}

DPLProcessResult DPLUDump::addCodePoint(char32_t cp) {
	if (encodeUtf8) {
		char encoded[] = "\\u0000";
		DPLUCppUtils::toStringBase(encoded, 16, cp);
		cout << encoded;
	} else {
		cout << (char) cp;
	}

	return DPL_PROCESS_ACCEPT;
}

bool DPLUString::isAvail() {
	return ptr < str.length();
}

char DPLUString::getNext() {
	return isAvail() ? str[ptr++] : 0;
}

char32_t DPLUString::getNextCodePoint() {
	return DPLUCppUtils::getNextCodePoint(this);
}

DPLProcessResult DPLUString::addCodePoint(char32_t cp) {
	return DPL_PROCESS_ACCEPT;
}

void DPLUString::process(string source, DPLUCodepointTarget *pTarget) {
	this->str = source;
}

string DPLUString::getString() {
	return str;
}

DPLUStream::DPLUStream(const char *fName) :
		inStream(fName) {

}

DPLUStream::~DPLUStream() {
	if (inStream.is_open()) {
		inStream.close();
	}
}

bool DPLUStream::isAvail() {
	return inStream.is_open() && !inStream.eof();
}

char DPLUStream::getNext() {
	char c;
	return inStream.get(c) ? c : 0;
}

char32_t DPLUStream::getNextCodePoint() {
	return DPLUCppUtils::getNextCodePoint(this);
}

void DPLUStream::process(const char* fName, DPLUCharTarget *target) {
	fstream inStream(fName);

	char c;
	while (inStream.get(c)) {
		target->addChar(c);
	}

	inStream.close();
}

void DPLUStream::process(const char* fName, DPLUCodepointTarget *target) {
	DPLUStream ds(fName);

	while (!ds.inStream.eof()) {
		target->addCodePoint(ds.getNextCodePoint());
	}
}

class DPLActionNull : public DPLNativeLogic {
public:
	virtual DPLProcessResult dplActionExecute() {
		return DPL_PROCESS_ACCEPT;
	}
};

DPLActionNull NULL_ACTION;

DPLNativeLogic *DPLUtils::getNullLogic() {
	return &NULL_ACTION;
}
