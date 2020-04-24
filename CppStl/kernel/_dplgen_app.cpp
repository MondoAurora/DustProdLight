#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_text.h>
#include <_dplgen_module_json.h>
#include <_dplgen_module_test01.h>

using namespace DPLUnitNarrative;
using namespace DPLUnitModel;
using namespace DPLUnitDialog;
using namespace DPLUnitDust;
using namespace DPLUnitTools;

using namespace DPLUnitJson;

using namespace DPLUnitTest01;

DPLEntity initTest01() {
	DPLEntity eProc = DPLData::createEntity(AgentHelloWorldSimple);

	DPLEntity eSep = DPLData::createEntity(AgentDump);
	DPLData::setString(eSep, DPLUnitText::AttTextString, "\n=======================\n");

	DPLEntity eMain = DPLData::createEntity(AgentRepeat);
	DPLData::setInt(eMain, DPLUnitTools::AttLimitsIntMax, 5);
	DPLData::setRef(eMain, DPLUnitTools::RefLinkTarget, eProc);
	DPLData::setRef(eMain, DPLUnitTools::RefCollectionSeparator, eSep);

	return eMain;
}

DPLEntity initFileDump(string fileName) {
	DPLEntity eInput = DPLData::createEntity(AgentReadStream);
	DPLData::setString(eInput, AttStreamURL, fileName);

	DPLEntity eDump = DPLData::createEntity(AgentDump);

	DPLEntity eDialog = DPLData::createEntity(TypeDialog);
	DPLData::setRef(eDialog, RefCollectionMembers, eInput);
	DPLData::setRef(eDialog, RefCollectionMembers, eDump);

	return eDialog;
}

DPLEntity parseJson(string fileName) {
	DPLEntity eInput = DPLData::createEntity(AgentReadStream);
	DPLData::setString(eInput, AttStreamURL, fileName);

//	DPLEntity eProc = DPLData::createEntity(AgentDump);
	DPLEntity eProc = DPLData::createEntity(AgentJsonParser);

	DPLEntity eDialog = DPLData::createEntity(TypeDialog);
	DPLData::setRef(eDialog, RefCollectionMembers, eInput);
	DPLData::setRef(eDialog, RefCollectionMembers, eProc);

	return eDialog;
}

void DPLMain::init() {
	DPLMain::registerModule("DplStl", DPLModuleDplStl::Module);
	DPLMain::registerModule("Text", DPLModuleText::Module);
	DPLMain::registerModule("Json", DPLModuleJson::Module);
	DPLMain::registerModule("Test01", DPLModuleTest01::Module);

	DPLEntity eMain;

//	eMain = initTest01();

	eMain = initFileDump("test1.json");

//	eMain = parseJson("test1.json");

	DPLData::setRef(DPL_CTX_RUNTIME, RefRuntimeMain, eMain);
}

void DPLMain::shutdown() {
	DPLModuleDplStl::Module->release();
	DPLModuleText::Module->release();
	DPLModuleJson::Module->release();
	DPLModuleTest01::Module->release();
}

