#include "_dplgen_app.h"

using namespace DPLUnitNarrative;
using namespace DPLUnitModel;
using namespace DPLUnitDialog;
using namespace DPLUnitDust;
using namespace DPLUnitTools;

using namespace DPLUnitJson;

using namespace DPLUnitTest01;

void DPLMain::init() {
	DPLMain::registerModule("DplStl", DPLModuleDplStl::Module);
	DPLMain::registerModule("Text", DPLModuleText::Module);
	DPLMain::registerModule("Json", DPLModuleJson::Module);
	DPLMain::registerModule("Test01", DPLModuleTest01::Module);

	DPLData::setRef(AgentRuntime, RefRuntimeMain, getMainEntity());
}

void DPLMain::shutdown() {
	DPLModuleDplStl::Module->release();
	DPLModuleText::Module->release();
	DPLModuleJson::Module->release();
	DPLModuleTest01::Module->release();
}

