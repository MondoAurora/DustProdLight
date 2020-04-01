/*
 * test.cpp
 *
 *  Created on: Feb 12, 2020.
 *      Author: Lorand Kedves
 */

#include <iostream>

#include <_dplgen_module_dpl_stl.h>
#include <temp/dplujson.h>
#include "test01.h"

using namespace std;


DPLProcessResult HelloWorldSimple::dplProcess() {
	cout << "Hello, world! from test01";

	return DPL_PROCESS_SUCCESS;
}

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
	DPLEntity e = DPLData::createEntity(DPLUnitTest01::ShapePath);

	DPLData::setString(e, DPLUnitTest01::IdName, "Room 01 töűk");
	DPLData::setInt(e, DPLUnitTest01::IdNum, e);
	DPLData::setDouble(e, DPLUnitTest01::VectorX, 3.14);
	DPLData::setDouble(e, DPLUnitTest01::VectorY, 9.81);

	addPoint(e, 88, 33);
	addPoint(e, 188, 33);
	addPoint(e, 188, 133);
	addPoint(e, 88, 133);

	cout << DPLData::getString(e, DPLUnitTest01::IdName, "??") << ": "
			<< DPLData::getInt(e, DPLUnitTest01::IdNum, -100) << " ("
			<< DPLData::getDouble(e, DPLUnitTest01::VectorX, -1) << ", "
			<< DPLData::getDouble(e, DPLUnitTest01::VectorY, -1) << ")" << endl;

	int pc = DPLData::getRefCount(e, DPLUnitTest01::ShapePath);
	for (int i = 0; i < pc; ++i) {
		DPLEntity pt = DPLData::getRef(e, DPLUnitTest01::ShapePath, i);
		cout << "   (" << DPLData::getDouble(pt, DPLUnitTest01::VectorX, -1) << ", "
				<< DPLData::getDouble(pt, DPLUnitTest01::VectorY, -1)
				<< ")" << endl;
	}

	return e;
}

void testProc() {
	cout << endl << "-- testProc --" << endl;

	DPLEntity eProc = DPLData::createEntity(DPLUnitTest01::Test);
	DPLData::setRef(eProc, DPLUnitModel::RefEntityActions,
			DPLUnitTools::ActionDump, DPLUnitNarrative::CmdProcess);

	DPLEntity eMsg;

	eMsg = DPLData::getRef(eProc, DPLUnitNarrative::CmdProcess);
	DPLData::setString(eMsg, DPLUnitText::AttTextString, "Hello world!");

	eMsg = DPLData::getRef(eProc, DPLUnitNarrative::CmdProcess);
	DPLData::setString(eMsg, DPLUnitText::AttTextString, "Hello world! (2)");

	DPLMain::signal(DPL_SIGNAL_OVER);
}

void testSequence() {
	cout << endl << "-- testSequence --" << endl;

	DPLEntity eDump = DPLData::createEntity(DPLUnitTools::TypeStream);
	DPLData::setRef(eDump, DPLUnitModel::RefEntityActions, DPLUnitTools::ActionDump, DPLUnitNarrative::CmdProcess);
	DPLData::setString(eDump, DPLUnitText::AttTextString, "Hello world!");

	DPLEntity eSequence = DPLData::createEntity(DPLUnitDust::TypeBlock);
	DPLData::setRef(eSequence, DPLUnitModel::RefEntityActions, DPLUnitNarrative::ActionSequence, DPLUnitNarrative::CmdProcess);
	DPLData::setRef(eSequence, DPLUnitTools::RefCollectionMembers, eDump);
	DPLData::setRef(eSequence, DPLUnitTools::RefCollectionMembers, eDump);

	DPLData::getRef(eSequence, DPLUnitNarrative::CmdProcess);

	DPLMain::signal(DPL_SIGNAL_OVER);
}

void testReadFile() {
	cout << endl << "-- testReadFile --" << endl;

	DPLEntity eStream = DPLData::createEntity(DPLUnitTools::TypeStream);
	DPLData::setString(eStream, DPLUnitTools::AttStreamURL, "test1.json");
	DPLData::setRef(eStream, DPLUnitModel::RefEntityActions, DPLUnitTools::ActionReadStream, DPLUnitNarrative::CmdProcess);

	DPLEntity eDump = DPLData::createEntity(DPLUnitTools::TypeStream);
	DPLData::setRef(eDump, DPLUnitModel::RefEntityActions, DPLUnitTools::ActionDump, DPLUnitNarrative::CmdProcess);

	DPLEntity eSequence = DPLData::createEntity(DPLUnitDust::TypeBlock);
	DPLData::setRef(eSequence, DPLUnitModel::RefEntityActions, DPLUnitNarrative::ActionSequence, DPLUnitNarrative::CmdProcess);
	DPLData::setRef(eSequence, DPLUnitTools::RefCollectionMembers, eStream);
	DPLData::setRef(eSequence, DPLUnitTools::RefCollectionMembers, eDump);

	DPLEntity eRepeat = DPLData::createEntity(DPLUnitDust::TypeBlock);
	DPLData::setRef(eRepeat, DPLUnitModel::RefEntityActions, DPLUnitNarrative::ActionRepeat, DPLUnitNarrative::CmdProcess);
	DPLData::setRef(eRepeat, DPLUnitTools::RefLinkTarget, eSequence);

	DPLData::getRef(eRepeat, DPLUnitNarrative::CmdProcess);

	DPLMain::signal(DPL_SIGNAL_OVER);
}

