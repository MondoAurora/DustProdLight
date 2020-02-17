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
	if ( strlen(CODE) < base ) {
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

//
//unsigned char CPCHK_BIT[] = { 7, 5, 4 };
//unsigned char CPCHK_MASK[] = { 0x1f, 0x0f, 0x07 };
//
//bool DPLUCodepointReader::optAppend() {
//	if (0 == step) {
//		head = chrSrc->getNext();
//		codePoint = head;
//	}
//
//	bool ret = (step < 4) && DPLUCppUtils::testBit(head, CPCHK_BIT[step]);
//
//	if (ret) {
//		codePoint = (codePoint << 6) + (chrSrc->getNext() & 0x3f);
//		char32_t headMask = CPCHK_MASK[step] << (6 * (++step));
//		codePoint = codePoint && headMask;
//	} else {
//		step = 0;
//	}
//
//	return ret;
//}
//
//char32_t DPLUCodepointReader::getNextCodePoint() {
//	while (optAppend())
//		;
//
//	return codePoint;
//}

char32_t DPLUCppUtils::getCodePoint(DPLUCharSource *chrSrc) {
	char32_t codePoint = 0;
	unsigned char chr = chrSrc->getNext();

	char32_t ret = chr;

	if (testBit(chr, 7)) // First byte>127, beyond the ASCII range.
			{
		if (testBit(chr, 5)) // First byte > 191, and so it must be at least a three-octet code point.
				{
			if (testBit(chr, 4)) // First byte > 224, and so it must be a four-octet code point.
					{
				codePoint = (chr & 0x07) << 18;
				chr = chrSrc->getNext();
				codePoint += (chr & 0x3f) << 12;
				chr = chrSrc->getNext();
				codePoint += (chr & 0x3f) << 6;
				chr = chrSrc->getNext();
				codePoint += (chr & 0x3f);
			} else {
				codePoint = (chr & 0x0f) << 12;
				chr = chrSrc->getNext();
				codePoint += (chr & 0x3f) << 6;
				chr = chrSrc->getNext();
				codePoint += (chr & 0x3f);
			}
		} else {
			ret = collectValue(0, 0, 0x1f, chr);
			ret = collectValue(ret, 6, 0x3f, chr);

			codePoint = (chr & 0x1f) << 6;
			chr = chrSrc->getNext();
			codePoint += (chr & 0x3f);
		}
	} else {
		codePoint = chr;
		ret = chr;
	}

	return codePoint;
}

unsigned char DPLUString::getNext() {
	return ptr < str.length() ? str[ptr++] : 0;
}

char32_t DPLUString::getNextCodePoint() {
//	return cpReader.getNextCodePoint();
	return DPLUCppUtils::getCodePoint(this);
}

void DPLUString::addCodePoint(char32_t cp) {
}

void DPLUString::process(string source, DPLUCodepointTarget *pTarget) {
	this->str = source;
}

string DPLUString::getString() {
	return str;
}
