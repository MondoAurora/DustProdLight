#include "text.h"

using namespace DPLUnitText;

class DPLModuleImpl: public DPLModule {
public:
	virtual ~DPLModuleImpl() {
	}

	virtual void init() {
		DPLData::setInt(ValCharCtrlBS, AttCharacterChar, 0x62);
		DPLData::setInt(ValCharCtrlCR, AttCharacterChar, 0x0D);
		DPLData::setInt(ValCharCtrlFF, AttCharacterChar, 0x0C);
		DPLData::setInt(ValCharCtrlLF, AttCharacterChar, 0x0A);
		DPLData::setInt(ValCharCtrlTAB, AttCharacterChar, 0x09);

		DPLData::setInt(ValCharQuot, AttCharacterChar, '\"');
		DPLData::setInt(ValCharSolidus, AttCharacterChar, '/');
		DPLData::setInt(ValCharReverseSolidus, AttCharacterChar, '\\');

		DPLMain::registerLogicProvider(this, LogicCharMatcher, LogicTokenMatcher, LogicNumberSci, LogicNumberRad,
				DPL_ENTITY_INVALID);
	}

	virtual DPLNarrativeLogic* createLogic(int logicId) const {
		if (LogicCharMatcher == logicId) {
			return new CharMatcher();
		} else if (LogicTokenMatcher == logicId) {
			return new TokenMatcher();
		} else if (LogicNumberSci == logicId) {
			return new NumberSci();
		} else if (LogicNumberRad == logicId) {
			return new NumberRad();
		}
		return NULL;
	}

	virtual void releaseLogic(int logicId, DPLNarrativeLogic* pLogic) const {
		if (LogicCharMatcher == logicId) {
			delete (CharMatcher*) pLogic;
		} else if (LogicTokenMatcher == logicId) {
			delete (TokenMatcher*) pLogic;
		} else if (LogicNumberSci == logicId) {
			delete (NumberSci*) pLogic;
		} else if (LogicNumberRad == logicId) {
			delete (NumberRad*) pLogic;
		}
	}

	virtual void release() {
	}
};

DPLModule* DPLModuleText::Module = new DPLModuleImpl();
