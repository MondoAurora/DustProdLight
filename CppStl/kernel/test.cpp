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

	p = DPL::createEntity(MapMeta::Position);

	DPL::setDouble(p, MapMeta::VectorX, x);
	DPL::setDouble(p, MapMeta::VectorY, y);

	DPL::setRef(shape, MapMeta::ShapePath, p, REFKEY_ARR_APPEND);

	bool odd = DPL::getRefCount(shape, MapMeta::ShapePath) % 2;
	DPL::setBool(p, MapMeta::TestOdd, odd);

}

int test() {
	DPLEntity e = DPL::createEntity(MapMeta::ShapePath);

	DPL::setString(e, MapMeta::IdName, "Room 01 töűk");
	DPL::setInt(e, MapMeta::IdNum, e);
	DPL::setDouble(e, MapMeta::VectorX, 3.14);
	DPL::setDouble(e, MapMeta::VectorY, 9.81);

	addPoint(e, 88, 33);
	addPoint(e, 188, 33);
	addPoint(e, 188, 133);
	addPoint(e, 88, 133);

	cout << DPL::getString(e, MapMeta::IdName, "??") << ": " << DPL::getInt(e, MapMeta::IdNum, -100) << " ("
			<< DPL::getDouble(e, MapMeta::VectorX, -1) << ", " << DPL::getDouble(e, MapMeta::VectorY, -1) << ")" << endl;

	int pc = DPL::getRefCount(e, MapMeta::ShapePath);
	for (int i = 0; i < pc; ++i) {
		DPLEntity pt = DPL::getRef(e, MapMeta::ShapePath, i);
		cout << "   (" << DPL::getDouble(pt, MapMeta::VectorX, -1) << ", " << DPL::getDouble(pt, MapMeta::VectorY, -1)
				<< ")" << endl;
	}

	return e;
}

int main() {
	DPLEntity e = test();

	DPLUEntityToJSON dumper(cout, true);
	DPL::visit(e, &dumper, NULL);

	DPLUStringCollector sc;

	DPLUStream::process("test2.json", &sc);

	cout << sc.getString() << endl;

	DPL::shutdown();

	return 0;
}

