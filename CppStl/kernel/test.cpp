/*
 * test.cpp
 *
 *  Created on: Feb 12, 2020.
 *      Author: Lorand Kedves
 */

#include <iostream>

#include "maptest.h"

using namespace std;

void addPoint(DPLEntity shape, double x, double y) {
	DPLEntity p;

	p = DPL::create_entity(MapMeta::Position);

	DPL::set_double(p, MapMeta::Vector_X, x);
	DPL::set_double(p, MapMeta::Vector_Y, y);

	DPL::set_ref(shape, MapMeta::Shape_Path, p, REFKEY_ARR_APPEND);
}

int test() {
	DPLEntity e = DPL::create_entity(MapMeta::Shape_Path);

	DPL::set_string(e, MapMeta::ID_Name, "Room 01");
	DPL::set_int(e, MapMeta::ID_id, e);
	DPL::set_double(e, MapMeta::Vector_X, 3.14);
	DPL::set_double(e, MapMeta::Vector_Y, 9.81);

	addPoint(e, 88, 33);
	addPoint(e, 188, 33);
	addPoint(e, 188, 133);
	addPoint(e, 88, 133);

	cout << DPL::get_string(e, MapMeta::ID_Name, "??") << ": " << DPL::get_int(e, MapMeta::ID_id, -100) << " ("
			<< DPL::get_double(e, MapMeta::Vector_X, -1) << ", " << DPL::get_double(e, MapMeta::Vector_Y, -1) << ")" << endl;

	int pc = DPL::get_ref_count(e, MapMeta::Shape_Path);
	for (int i = 0; i < pc; ++i) {
		DPLEntity pt = DPL::get_ref(e, MapMeta::Shape_Path, i);
		cout << "   (" << DPL::get_double(pt, MapMeta::Vector_X, -1) << ", " << DPL::get_double(pt, MapMeta::Vector_Y, -1)
				<< ")" << endl;
	}

	return 0;
}

int main() {
	test();

	DPL::shutdown();

	return 0;
}

