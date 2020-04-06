/*
 * test.cpp
 *
 *  Created on: Feb 12, 2020.
 *      Author: Lorand Kedves
 */

#include <iostream>

#include "dpl.h"

int main() {
	cout << endl << "-- DPL init --" << endl;

	DPLMain::init();

	cout << endl << "-- DPL run --" << endl;

	DPLMain::run();

	cout << endl << "-- DPL shutdown --" << endl;

	DPLMain::shutdown();

	cout << endl << "-- DPL done --" << endl;

	return 0;
}

