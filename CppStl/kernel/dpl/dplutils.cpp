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

const char* BASE_CODE = "0123456789abcdef";
const int BASE_COUNT = strlen(BASE_CODE);

bool DPLUCppUtils::toStringBase(char* target, unsigned char base, unsigned int value) {
	if (BASE_COUNT < base) {
		return false;
	}

	int idx = strlen(target);

	for (unsigned int rem = value; idx && rem; rem = rem / base) {
		target[--idx] = BASE_CODE[rem % base];
	}

	return idx;
}

unsigned int DPLUCppUtils::addByBase(unsigned int &target, unsigned char base, unsigned char value) {
	if (BASE_COUNT < base) {
		return target;
	}

	const char * end = BASE_CODE + BASE_COUNT;
	const char * p = find(BASE_CODE, end, value);
	if (p < end) {
		target = target * base + (p - BASE_CODE);
	}

	return target;

}

bool DPLUCppUtils::testBit(unsigned int value, unsigned int bit) {
	unsigned int chk = 1 << bit;
	return value & chk;
}

//unsigned int DPLUCppUtils::collectValue(unsigned int target, unsigned int shift, unsigned int mask, unsigned int val) {
//	return (target << shift) + (val & mask);
//}

unsigned char CPCHK_BIT[] = { 7, 5, 4 };
unsigned char CPCHK_MASK[] = { 0x1f, 0x0f, 0x07 };

char32_t DPLUCppUtils::getNextCodePoint(DPLUCharSource *chrSrc) {
	unsigned int step = 0;
	unsigned char head = chrSrc->getNext();
	int headLead = 0;
	char32_t codePoint = 0;

	while ((step < 4) && DPLUCppUtils::testBit(head, CPCHK_BIT[step++]) ) {
		codePoint = (codePoint << 6) + (chrSrc->getNext() & 0x3f);
		headLead += 6;
	}

	if ( 0 == headLead ) {
		codePoint = head;
	} else {
		codePoint += (head & CPCHK_MASK[step]) << headLead;
	}

	return codePoint;
}

DPLProcessResponse DPLUDump::addCodePoint(char32_t cp) {
	if ( encodeUtf8 ) {
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


unsigned char DPLUString::getNext() {
	return isAvail() ? str[ptr++] : 0;
}

char32_t DPLUString::getNextCodePoint() {
	return DPLUCppUtils::getNextCodePoint(this);
}

DPLProcessResponse DPLUString::addCodePoint(char32_t cp) {
	return DPL_PROCESS_ACCEPT;
}

void DPLUString::process(string source, DPLUCodepointTarget *pTarget) {
	this->str = source;
}

string DPLUString::getString() {
	return str;
}

DPLUStream::DPLUStream(const char *fName) : inStream(fName) {

}

DPLUStream::~DPLUStream() {
	if (inStream.is_open() ) {
		inStream.close();
	}
}

bool DPLUStream::isAvail() {
	return inStream.is_open() && !inStream.eof();
}

unsigned char DPLUStream::getNext() {
  char c;
  return inStream.get(c) ? c : 0;
}

char32_t DPLUStream::getNextCodePoint() {
	return DPLUCppUtils::getNextCodePoint(this);
}


void DPLUStream::process(const char* fName, DPLUCodepointTarget *target) {
	DPLUStream ds(fName);

	while ( !ds.inStream.eof() ) {
		target->addCodePoint(ds.getNextCodePoint());
	}
}
