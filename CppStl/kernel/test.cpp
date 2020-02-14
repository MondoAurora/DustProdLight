/*
 * test.cpp
 *
 *  Created on: Feb 12, 2020.
 *      Author: Lorand Kedves
 */

#include <iostream>
using namespace std;

#include "maptest.h"

class DPLTest {
	MapMeta meta;

	void addPoint(DPLEntity shape, double x, double y) {
		DPLEntity p;

		p = DPLStore::createEntity(meta.Position);

		DPLStore::set_double(p, meta.Vector_X, x);
		DPLStore::set_double(p, meta.Vector_Y, y);

		DPLStore::set_ref(shape, meta.Shape_Path, p, REFKEY_ARR_APPEND);
	}

public:
	int test() {
		DPLEntity e = DPLStore::createEntity(meta.Shape_Path);

		DPLStore::set_string(e, meta.ID_Name, "Room 01");
		DPLStore::set_int(e, meta.ID_id, e);
		DPLStore::set_double(e, meta.Vector_X, 3.14);
		DPLStore::set_double(e, meta.Vector_Y, 666);

		addPoint(e, 88, 33);
		addPoint(e, 188, 33);
		addPoint(e, 188, 133);
		addPoint(e, 88, 133);

		cout << DPLStore::get_string(e, meta.ID_Name, "??") << ": "
				<< DPLStore::get_int(e, meta.ID_id, -100) << " ("
				<< DPLStore::get_double(e, meta.Vector_X, -1) << ", "
				<< DPLStore::get_double(e, meta.Vector_Y, -1) << ")" << endl;

		int pc = DPLStore::get_ref_count(e, meta.Shape_Path);
		for (int i = 0; i < pc; ++i) {
			DPLEntity pt = DPLStore::get_ref(e, meta.Shape_Path, i);
			cout << "   (" << DPLStore::get_double(pt, meta.Vector_X, -1) << ", "
					<< DPLStore::get_double(pt, meta.Vector_Y, -1) << ")" << endl;
		}

		return 0;
	}
};

int main() {
	DPLTest test;

	test.test();

	return 0;
}

