/*
 * dpl.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_dpl_text.h>
#include <_dplgen_module_test01.h>

using namespace DPLUnitNarrative;
using namespace DPLUnitModel;
using namespace DPLUnitDialog;
using namespace DPLUnitTest01;
using namespace DPLUnitDust;
using namespace DPLUnitTools;

DPLEntity initTest01() {
	DPLEntity eProc = DPLData::createEntity(TypeHelloWorldSimple);
	DPLData::setRef(eProc, RefEntityActions, ActionHelloWorldSimple, CmdActionExecute);

	DPLEntity eSep = DPLData::createEntity(Test);
	DPLData::setRef(eSep, RefEntityActions, ActionDump, CmdActionExecute);
	DPLData::setString(eSep, DPLUnitText::AttTextString, "\n=======================\n");

	DPLEntity eMain = DPLData::createEntity(TypeCtrlRepeat);
	DPLData::setRef(eMain, RefEntityActions, ActionCtrlRepeat, CmdActionExecute);
	DPLData::setInt(eMain, DPLUnitTools::AttLimitsIntMax, 5);
	DPLData::setRef(eMain, DPLUnitTools::RefLinkTarget, eProc);
	DPLData::setRef(eMain, DPLUnitTools::RefCollectionSeparator, eSep);

	return eMain;
}

DPLEntity initFileDump() {
	DPLEntity eInput = DPLData::createEntity(Test);
	DPLData::setString(eInput, AttStreamURL, "test1.json");
	DPLData::setRef(eInput, RefEntityActions, ActionReadStream, CmdActionExecute);

	DPLEntity eDump = DPLData::createEntity(Test);
	DPLData::setRef(eDump, RefEntityActions, ActionDump, CmdActionExecute);

	DPLEntity eDialog = DPLData::createEntity(TypeDialog);
	DPLData::setRef(eDialog, RefCollectionMembers, eInput);
	DPLData::setRef(eDialog, RefCollectionMembers, eDump);

	return eDialog;
}

void DPLMain::init() {
	DPLModuleDplStl::Module->init();
	DPLModuleTest01::Module->init();

	DPLEntity eMain;

//	eMain = initTest01();

	eMain = initFileDump();

	DPLData::setRef(DPL_CTX_RUNTIME, RefRuntimeMain, eMain);
}

void DPLMain::shutdown() {
	DPLModuleDplStl::Module->release();
	DPLModuleTest01::Module->release();
}

