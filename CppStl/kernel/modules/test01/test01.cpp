#include <iostream>

#include <_dplgen_module_dpl_stl.h>
#include <temp/dplujson.h>
#include "test01.h"

using namespace std;

void addPoint(DPLEntity shape, double x, double y) {
	DPLEntity p;

	p = DPLData::createEntity(DPLUnitTest01::Position);

	DPLData::setDouble(p, DPLUnitTest01::VectorX, x);
	DPLData::setDouble(p, DPLUnitTest01::VectorY, y);

	DPLData::setRef(shape, DPLUnitTest01::ShapePath, p, DPL_REFKEY_ARR_APPEND);

	bool odd = DPLData::getRefCount(shape, DPLUnitTest01::ShapePath) % 2;
	DPLData::setBool(p, DPLUnitTest01::TestOdd, odd);
}

int test() {
	cout << endl << "-- creating main entity --" << endl;

	DPLEntity e = DPLData::createEntity(DPLUnitTest01::ShapePath);

	DPLData::setString(e, DPLUnitTest01::IdName, "Room 01 töűk");
	DPLData::setInt(e, DPLUnitTest01::IdNum, e);
	DPLData::setDouble(e, DPLUnitTest01::VectorX, 3.14);
	DPLData::setDouble(e, DPLUnitTest01::VectorY, 9.81);

	addPoint(e, 88, 33);
	addPoint(e, 188, 33);
	addPoint(e, 188, 133);
	addPoint(e, 88, 133);

	cout << DPLData::getString(e, DPLUnitTest01::IdName, "??") << ": " << DPLData::getInt(e, DPLUnitTest01::IdNum, -100)
			<< " (" << DPLData::getDouble(e, DPLUnitTest01::VectorX, -1) << ", "
			<< DPLData::getDouble(e, DPLUnitTest01::VectorY, -1) << ")" << endl;

	int pc = DPLData::getRefCount(e, DPLUnitTest01::ShapePath);
	for (int i = 0; i < pc; ++i) {
		DPLEntity pt = DPLData::getRef(e, DPLUnitTest01::ShapePath, i);
		cout << "   (" << DPLData::getDouble(pt, DPLUnitTest01::VectorX, -1) << ", "
				<< DPLData::getDouble(pt, DPLUnitTest01::VectorY, -1) << ")" << endl;
	}

	return e;
}

void dump(DPLEntity e) {
	cout << endl << "-- dump --" << endl;

	DPLUEntityToJSON dumper(cout, true);
	DPLData::visit(e, &dumper, NULL);
}

DPLProcessResult HelloWorldSimple::dplActionExecute() {
	cout << "Hello, world! from test01 " << count++ << endl;

	if (!e) {
		e = test();
	}

	dump(e);

	return DPL_PROCESS_ACCEPT;
//	return (++count < 5) ? DPL_PROCESS_ACCEPT : DPL_PROCESS_SUCCESS;
}
