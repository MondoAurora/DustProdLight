/*
 * test.cpp
 *
 *  Created on: Feb 12, 2020.
 *      Author: Lorand Kedves
 */

#include <iostream>

#include "maptest.h"

#include "dpl/dplujson.h"

using namespace std;

void addPoint(DPLEntity shape, double x, double y) {
	DPLEntity p;

	p = DPLData::createEntity(MapMeta::Position);

	DPLData::setDouble(p, MapMeta::VectorX, x);
	DPLData::setDouble(p, MapMeta::VectorY, y);

	DPLData::setRef(shape, MapMeta::ShapePath, p, REFKEY_ARR_APPEND);

	bool odd = DPLData::getRefCount(shape, MapMeta::ShapePath) % 2;
	DPLData::setBool(p, MapMeta::TestOdd, odd);

}

int test() {
	DPLEntity e = DPLData::createEntity(MapMeta::ShapePath);

	DPLData::setString(e, MapMeta::IdName, "Room 01 töűk");
	DPLData::setInt(e, MapMeta::IdNum, e);
	DPLData::setDouble(e, MapMeta::VectorX, 3.14);
	DPLData::setDouble(e, MapMeta::VectorY, 9.81);

	addPoint(e, 88, 33);
	addPoint(e, 188, 33);
	addPoint(e, 188, 133);
	addPoint(e, 88, 133);

	cout << DPLData::getString(e, MapMeta::IdName, "??") << ": "
			<< DPLData::getInt(e, MapMeta::IdNum, -100) << " ("
			<< DPLData::getDouble(e, MapMeta::VectorX, -1) << ", "
			<< DPLData::getDouble(e, MapMeta::VectorY, -1) << ")" << endl;

	int pc = DPLData::getRefCount(e, MapMeta::ShapePath);
	for (int i = 0; i < pc; ++i) {
		DPLEntity pt = DPLData::getRef(e, MapMeta::ShapePath, i);
		cout << "   (" << DPLData::getDouble(pt, MapMeta::VectorX, -1) << ", "
				<< DPLData::getDouble(pt, MapMeta::VectorY, -1)
				<< ")" << endl;
	}

	return e;
}

int main() {
	DPLEntity e = test();

	DPLUEntityToJSON dumper(cout, true);
	DPLData::visit(e, &dumper, NULL);

	cout << endl << "----" << endl;

	DPLJson::read("test1.json");

	cout << endl << "----" << endl;

	DPLMain::shutdown();

	return 0;
}

