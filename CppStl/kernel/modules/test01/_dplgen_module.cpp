
#include "test01.h"

using namespace DPLUnitTest01;

class DPLModuleTest: public DPLModule {
public:
	virtual ~DPLModuleTest() {

	}

	virtual void init() {
		DPLData::setRef(ActionHelloWorldSimple, DPLUnitTools::RefConnectedExtends, DPLUnitNarrative::SvcAction);

		DPLData::setRef(TypeHelloWorldSimple, DPLUnitIdea::RefTypeDefaultActions, ActionHelloWorldSimple);

		DPLMain::registerLogicProvider(this,
				ActionHelloWorldSimple,
				DPL_ENTITY_INVALID);
	}

	virtual DPLAction* createLogic(int logicId) const {
			if (ActionHelloWorldSimple == logicId) {
				return new HelloWorldSimple();
			}

		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLAction* pLogic) const {
			if (ActionHelloWorldSimple == logicId) {
				delete (HelloWorldSimple*) pLogic;
			}
	}

	virtual void release() {
	}
};

DPLModule* DPLModuleTest01::Module = new DPLModuleTest();

