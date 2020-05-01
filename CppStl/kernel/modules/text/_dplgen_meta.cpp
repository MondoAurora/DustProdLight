#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_text.h>

using namespace DPLUnitText;

const DPLEntity DPLUnitText::UnitText = DPLData::getMetaEntity(DPLUnitModel::TypeUnit, "Text");

const DPLEntity DPLUnitText::TypeCharacter = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Character", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttCharacterChar = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Char", DPLUnitText::TypeCharacter);

const DPLEntity DPLUnitText::TypeTextPlain = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "TextPlain", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttTextString = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "String", DPLUnitText::TypeTextPlain);

const DPLEntity DPLUnitText::TypeTextNumber = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Number", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttNumFormat = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Format", DPLUnitText::TypeTextNumber);
const DPLEntity DPLUnitText::AttNumRadix = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Radix", DPLUnitText::TypeTextNumber);

const DPLEntity DPLUnitText::ValCharQuot = DPLData::getMetaEntity(TypeCharacter, "CharQuot", UnitText);
const DPLEntity DPLUnitText::ValCharSolidus = DPLData::getMetaEntity(TypeCharacter, "CharSolidus", UnitText);
const DPLEntity DPLUnitText::ValCharReverseSolidus = DPLData::getMetaEntity(TypeCharacter, "CharReverseSolidus", UnitText);

const DPLEntity DPLUnitText::ValCharCtrlTAB = DPLData::getMetaEntity(TypeCharacter, "CtrlTAB", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlLF = DPLData::getMetaEntity(TypeCharacter, "CtrlLF", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlCR = DPLData::getMetaEntity(TypeCharacter, "CtrlCR", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlFF = DPLData::getMetaEntity(TypeCharacter, "CtrlFF", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlBS = DPLData::getMetaEntity(TypeCharacter, "CtrlBS", UnitText);

const DPLEntity DPLUnitText::AgentCharMatcher = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "CharMatcher", UnitText);
const DPLEntity DPLUnitText::AgentTokenMatcher = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "TokenMatcher", UnitText);
const DPLEntity DPLUnitText::AgentNumberSci = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "NumberSci", UnitText);
const DPLEntity DPLUnitText::AgentNumberRad = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "NumberRad", UnitText);
