
#include "maptest.h"

using namespace MapMeta;

const DPLEntity MapMeta::TestUnit = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Test");
const DPLEntity MapMeta::Test = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Test", TestUnit);

const DPLEntity MapMeta::TestOdd = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "Odd", Test);

const DPLEntity MapMeta::Vector = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Vector", TestUnit);

const DPLEntity MapMeta::VectorX = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "X", Vector);
const DPLEntity MapMeta::VectorY = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "Y", Vector);

const DPLEntity MapMeta::Id = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Identified", TestUnit);

const DPLEntity MapMeta::IdName = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "Name", Id);
const DPLEntity MapMeta::IdNum = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Num", Id);

const DPLEntity MapMeta::Position = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Position", TestUnit);

const DPLEntity MapMeta::Shape = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Shape", TestUnit);

const DPLEntity MapMeta::ShapeDirection = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Direction", Shape);

const DPLEntity MapMeta::ShapePath = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Path", Shape);

const DPLEntity MapMeta::Moving = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Moving", TestUnit);

const DPLEntity MapMeta::MovingVelocity = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Velocity", Moving);
const DPLEntity MapMeta::MovingAcceleration = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Acceleration", Moving);
