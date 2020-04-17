
#include <_dplgen_module_text.h>

using namespace DPLUnitText;

const DPLEntity DPLUnitText::UnitText = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Text");
const DPLEntity DPLUnitText::TypeCharacter = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Character", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttCharacterChar = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Char", DPLUnitText::TypeCharacter);
const DPLEntity DPLUnitText::TypeTextPlain = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "TextPlain", DPLUnitText::UnitText);
const DPLEntity DPLUnitText::AttTextString = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "String", DPLUnitText::TypeTextPlain);
