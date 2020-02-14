/*
	header of strconv.cpp
	(Part of the 32 bit CAPNS library.)

	- conversions to and from string
*/
#ifndef STRCONV_H_
#define STRCONV_H_

#include <string>

using namespace std;

class StrConv{
	public:
	static	string int_to_str(const int i);
	static	int str_to_int(const string& s);
	static	string double_to_str(const double i);
	static	double str_to_double(const string& s);
};

#endif /*STRCONV_H_*/
