/*
	strconv.cpp
	(Part of the 32 bit CAPNS library.)

	- conversions to and from string
*/
#include "strconv.h"

#include <sstream>

using namespace std;

string StrConv::int_to_str(const int i){
	stringstream ss;
	string s;
	ss<<i;
	ss>>s;
	return s;
}

int StrConv::str_to_int(const string& s){
	stringstream ss;
	int i;
	ss<<s;
	ss>>i;
	return i;
}

string StrConv::double_to_str(const double d){
	stringstream ss;
	string s;
	ss<<d;
	ss>>s;
	return s;
}

double StrConv::str_to_double(const string& s){
	stringstream ss;
	double d;
	ss<<s;
	ss>>d;
	return d;
}
