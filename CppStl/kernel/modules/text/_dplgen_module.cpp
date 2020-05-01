#include "text.h"

using namespace DPLUnitText;

class DPLModuleTextImpl: public DPLModule {
public:
	virtual ~DPLModuleTextImpl() {
	}

	virtual void init(DPLEntity eModule) {
		DPLData::setInt(ValCharCtrlBS, AttCharacterChar, 0x62);
		DPLData::setInt(ValCharCtrlCR, AttCharacterChar, 0x0D);
		DPLData::setInt(ValCharCtrlFF, AttCharacterChar, 0x0C);
		DPLData::setInt(ValCharCtrlLF, AttCharacterChar, 0x0A);
		DPLData::setInt(ValCharCtrlTAB, AttCharacterChar, 0x09);

		DPLData::setInt(ValCharQuot, AttCharacterChar, '\"');
		DPLData::setInt(ValCharSolidus, AttCharacterChar, '/');
		DPLData::setInt(ValCharReverseSolidus, AttCharacterChar, '\\');

		DPLData::setRef(eModule, DPLUnitNative::RefModuleAgents, AgentCharMatcher);
		DPLData::setRef(eModule, DPLUnitNative::RefModuleAgents, AgentTokenMatcher);
		DPLData::setRef(eModule, DPLUnitNative::RefModuleAgents, AgentNumberSci);
		DPLData::setRef(eModule, DPLUnitNative::RefModuleAgents, AgentNumberRad);
	}

	virtual DPLNativeLogic* createLogic(int logicId) const {
		if (AgentCharMatcher == logicId) {
			return new CharMatcher();
		} else if (AgentTokenMatcher == logicId) {
			return new TokenMatcher();
		} else if (AgentNumberSci == logicId) {
			return new NumberSci();
		} else if (AgentNumberRad == logicId) {
			return new NumberRad();
		}
		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLNativeLogic* pLogic) const {
		if (AgentCharMatcher == logicId) {
			delete (CharMatcher*) pLogic;
		} else if (AgentTokenMatcher == logicId) {
			delete (TokenMatcher*) pLogic;
		} else if (AgentNumberSci == logicId) {
			delete (NumberSci*) pLogic;
		} else if (AgentNumberRad == logicId) {
			delete (NumberRad*) pLogic;
		}
	}

	virtual void release() {
	}
};

DPLModule* DPLModuleText::Module = new DPLModuleTextImpl();
