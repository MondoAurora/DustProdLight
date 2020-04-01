/*
 * dpl.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_test01.h>


DPLEntity DPLMain::init() {
	DPLModuleDplStl::Module->init();
	DPLModuleTest01::Module->init();

	DPLEntity eProc = DPLData::createEntity(DPLUnitTest01::Test);
	DPLData::setRef(eProc, DPLUnitModel::RefEntityActions, DPLUnitTest01::ActionHelloWorldSimple, DPLUnitNarrative::CmdProcess);

	return eProc;
}

DPLProcessResult DPLMain::tempRun(DPLEntity eProc) {
	DPLEntity eMsg;

	eMsg = DPLData::getRef(eProc, DPLUnitNarrative::CmdProcess);
	DPLData::setString(eMsg, DPLUnitText::AttTextString, "Hello world!");

	return DPLMain::signal(DPL_SIGNAL_OVER);
}

void DPLMain::shutdown() {
	DPLModuleDplStl::Module->release();
	DPLModuleTest01::Module->release();
}

