#include <_dplgen_module_text.h>

using namespace DPLUnitText;

const DPLEntity DPLUnitText::UnitText = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Text");

const DPLEntity DPLUnitText::TypeCharacter = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Character", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttCharacterChar = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Char", DPLUnitText::TypeCharacter);

const DPLEntity DPLUnitText::TypeTextPlain = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "TextPlain", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttTextString = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "String", DPLUnitText::TypeTextPlain);

const DPLEntity DPLUnitText::TypeTextNumber = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Number", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttNumFormat = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Format", DPLUnitText::TypeTextNumber);
const DPLEntity DPLUnitText::AttNumRadix = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Radix", DPLUnitText::TypeTextNumber);

const DPLEntity DPLUnitText::ValCharQuot = DPLData::getConst(TypeCharacter, "CharQuot", UnitText);
const DPLEntity DPLUnitText::ValCharSolidus = DPLData::getConst(TypeCharacter, "CharSolidus", UnitText);
const DPLEntity DPLUnitText::ValCharReverseSolidus = DPLData::getConst(TypeCharacter, "CharReverseSolidus", UnitText);

const DPLEntity DPLUnitText::ValCharCtrlTAB = DPLData::getConst(TypeCharacter, "CtrlTAB", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlLF = DPLData::getConst(TypeCharacter, "CtrlLF", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlCR = DPLData::getConst(TypeCharacter, "CtrlCR", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlFF = DPLData::getConst(TypeCharacter, "CtrlFF", UnitText);
const DPLEntity DPLUnitText::ValCharCtrlBS = DPLData::getConst(TypeCharacter, "CtrlBS", UnitText);

const DPLEntity DPLUnitText::AgentCharMatcher = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "CharMatcher", UnitText);
const DPLEntity DPLUnitText::AgentTokenMatcher = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "TokenMatcher", UnitText);
const DPLEntity DPLUnitText::AgentNumberSci = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "NumberSci", UnitText);
const DPLEntity DPLUnitText::AgentNumberRad = DPLData::getMetaEntity(DPL_TOKEN_AGENT, "NumberRad", UnitText);
