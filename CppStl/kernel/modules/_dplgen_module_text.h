#ifndef DPLGEN_MODULE_TEXT_H_
#define DPLGEN_MODULE_TEXT_H_

#include <dpl.h>

#include <_dplgen_module_dpl_stl.h>

namespace DPLUnitText {
extern const DPLEntity UnitText;

extern const DPLEntity TypeCharacter;
extern const DPLEntity AttCharacterChar;

extern const DPLEntity TypeTextPlain;
extern const DPLEntity AttTextString;

extern const DPLEntity TypeTextNumber;
extern const DPLEntity AttNumFormat;
extern const DPLEntity AttNumRadix;

extern const DPLEntity ValCharQuot;
extern const DPLEntity ValCharSolidus;
extern const DPLEntity ValCharReverseSolidus;

extern const DPLEntity ValCharCtrlTAB;
extern const DPLEntity ValCharCtrlLF;
extern const DPLEntity ValCharCtrlCR;
extern const DPLEntity ValCharCtrlFF;
extern const DPLEntity ValCharCtrlBS;

extern const DPLEntity LogicCharMatcher;
extern const DPLEntity LogicTokenMatcher;
extern const DPLEntity LogicNumberSci;
extern const DPLEntity LogicNumberRad;
}

namespace DPLModuleText {
extern DPLModule* Module;
}

#endif /*DPLGEN_MODULE_TEXT_H_*/
