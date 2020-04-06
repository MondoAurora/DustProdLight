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


void DPLMain::init() {
	DPLModuleDplStl::Module->init();
	DPLModuleTest01::Module->init();

	DPLEntity eProc = DPLData::createEntity(DPLUnitTest01::Test);
	DPLData::setRef(eProc, DPLUnitModel::RefEntityActions, DPLUnitTest01::ActionHelloWorldSimple, DPLUnitNarrative::CmdProcess);

	DPLEntity eTask = DPLData::createEntity(DPLUnitDialog::TypeTask);
	DPLData::setRef(eTask, DPLUnitDialog::RefTaskTarget, eProc);
	DPLData::setRef(eTask, DPLUnitDialog::RefTaskCommand, DPLUnitNarrative::CmdProcess);

	DPLData::setRef(DPL_CTX_RUNTIME, DPLUnitDust::RefRuntimeMain, eTask);
}

void DPLMain::shutdown() {
	DPLModuleDplStl::Module->release();
	DPLModuleTest01::Module->release();
}

