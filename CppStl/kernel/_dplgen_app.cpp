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

using namespace DPLUnitNarrative;
using namespace DPLUnitModel;
using namespace DPLUnitTest01;

void DPLMain::init() {
	DPLModuleDplStl::Module->init();
	DPLModuleTest01::Module->init();

	DPLEntity eProc = DPLData::createEntity(Test);
	DPLData::setRef(eProc, RefEntityActions, ActionHelloWorldSimple, CmdActionExecute);

	DPLEntity eTask = DPLData::createEntity(TypeExecAtom);
	DPLData::setRef(eTask, RefExecAtomTarget, eProc);
	DPLData::setRef(eTask, RefExecAtomCommand, DPLUnitNarrative::CmdActionExecute);

	DPLData::setRef(DPL_CTX_RUNTIME, DPLUnitDust::RefRuntimeMain, eTask);


//	DPLEntity eProc = DPLData::createEntity(DPLUnitTest01::TypeHelloWorldSimple);
//
//	DPLEntity eTask = DPLData::createEntity(DPLUnitNarrative::TypeExecAtom);
//	DPLData::setRef(eTask, DPLUnitNarrative::RefExecAtomTarget, eProc);
//
//	DPLEntity eMain = DPLData::createEntity(DPLUnitNarrative::TypeCtrlRepeat);
//	DPLData::setInt(eMain, DPLUnitTools::AttLimitsIntMax, 5);
//	DPLData::setRef(eMain, DPLUnitTools::RefCollectionMembers, eTask);
//
//	DPLData::setRef(DPL_CTX_RUNTIME, DPLUnitDust::RefRuntimeMain, eMain);
}

void DPLMain::shutdown() {
	DPLModuleDplStl::Module->release();
	DPLModuleTest01::Module->release();
}

