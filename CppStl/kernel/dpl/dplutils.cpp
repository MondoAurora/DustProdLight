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

char CODE[] = "0123456789abcdef";

bool DPLUCppUtils::toStringBase(char* target, unsigned char base, unsigned int value) {
	if (strlen(CODE) < base) {
		return false;
	}

	int idx = strlen(target);

	for (unsigned int rem = value; idx && rem; rem = rem / base) {
		target[--idx] = CODE[rem % base];
	}

	return idx;
}

bool DPLUCppUtils::testBit(unsigned int value, unsigned int bit) {
	unsigned int chk = 1 << bit;
	return value & chk;
}

unsigned int DPLUCppUtils::collectValue(unsigned int target, unsigned int shift, unsigned int mask, unsigned int val) {
	return (target << shift) + (val & mask);
}

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

unsigned char DPLUString::getNext() {
	return ptr < str.length() ? str[ptr++] : 0;
}

char32_t DPLUString::getNextCodePoint() {
	return DPLUCppUtils::getNextCodePoint(this);
}

void DPLUString::addCodePoint(char32_t cp) {
}

void DPLUString::process(string source, DPLUCodepointTarget *pTarget) {
	this->str = source;
}

string DPLUString::getString() {
	return str;
}
