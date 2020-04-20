#include "text.h"

using namespace DPLUnitText;

class DPLModuleImpl: public DPLModule {
public:
	virtual ~DPLModuleImpl() {
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

		DPLData::setRef(eModule, DPLUnitNative::RefModuleActions, AgentCharMatcher);
		DPLData::setRef(eModule, DPLUnitNative::RefModuleActions, AgentTokenMatcher);
		DPLData::setRef(eModule, DPLUnitNative::RefModuleActions, AgentNumberSci);
		DPLData::setRef(eModule, DPLUnitNative::RefModuleActions, AgentNumberRad);
	}

	virtual DPLNarrativeLogic* createLogic(int logicId) const {
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

	virtual void releaseLogic(int logicId, DPLNarrativeLogic* pLogic) const {
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

DPLModule* DPLModuleText::Module = new DPLModuleImpl();
