#include "test01.h"

using namespace DPLUnitTest01;

class DPLModuleTest: public DPLModule {
public:
	virtual ~DPLModuleTest() {

	}

	virtual void init(DPLEntity eModule) {
		DPLData::setRef(eModule, DPLUnitNative::RefModuleAgents, AgentHelloWorldSimple);
	}

	virtual DPLNativeLogic* createLogic(int logicId) const {
			if (AgentHelloWorldSimple == logicId) {
				return new HelloWorldSimple();
			}

		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLNativeLogic* pLogic) const {
			if (AgentHelloWorldSimple == logicId) {
				delete (HelloWorldSimple*) pLogic;
			}
	}

	virtual void release() {
	}
};

DPLModule* DPLModuleTest01::Module = new DPLModuleTest();

