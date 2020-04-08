
#include <_dplgen_module_test01.h>

using namespace DPLUnitTest01;

const DPLEntity DPLUnitTest01::TestUnit = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Test");
const DPLEntity DPLUnitTest01::Test = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Test", TestUnit);
const DPLEntity DPLUnitTest01::TypeHelloWorldSimple = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "HelloWorldSimple", TestUnit);

const DPLEntity DPLUnitTest01::TestOdd = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "Odd", Test);

const DPLEntity DPLUnitTest01::Vector = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Vector", TestUnit);

const DPLEntity DPLUnitTest01::VectorX = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "X", Vector);
const DPLEntity DPLUnitTest01::VectorY = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "Y", Vector);

const DPLEntity DPLUnitTest01::Id = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Identified", TestUnit);

const DPLEntity DPLUnitTest01::IdName = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "Name", Id);
const DPLEntity DPLUnitTest01::IdNum = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Num", Id);

const DPLEntity DPLUnitTest01::Position = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Position", TestUnit);

const DPLEntity DPLUnitTest01::Shape = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Shape", TestUnit);

const DPLEntity DPLUnitTest01::ShapeDirection = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Direction", Shape);

const DPLEntity DPLUnitTest01::ShapePath = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Path", Shape);

const DPLEntity DPLUnitTest01::Moving = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Moving", TestUnit);

const DPLEntity DPLUnitTest01::MovingVelocity = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Velocity", Moving);
const DPLEntity DPLUnitTest01::MovingAcceleration = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Acceleration", Moving);

const DPLEntity DPLUnitTest01::ActionHelloWorldSimple = DPLData::getMetaEntity(DPL_TOKEN_ACTION, "HelloWorldSimple", TestUnit);
