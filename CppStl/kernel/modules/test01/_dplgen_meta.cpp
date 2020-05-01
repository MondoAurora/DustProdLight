#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_test01.h>

using namespace DPLUnitTest01;

const DPLEntity DPLUnitTest01::TestUnit = DPLData::getMetaEntity(DPLUnitModel::TypeUnit, "Test");
const DPLEntity DPLUnitTest01::Test = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Test", TestUnit);

const DPLEntity DPLUnitTest01::TestOdd = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "Odd", Test);

const DPLEntity DPLUnitTest01::Vector = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Vector", TestUnit);

const DPLEntity DPLUnitTest01::VectorX = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "X", Vector);
const DPLEntity DPLUnitTest01::VectorY = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "Y", Vector);

const DPLEntity DPLUnitTest01::Id = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Identified", TestUnit);

const DPLEntity DPLUnitTest01::IdName = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "Name", Id);
const DPLEntity DPLUnitTest01::IdNum = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Num", Id);

const DPLEntity DPLUnitTest01::Position = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Position", TestUnit);

const DPLEntity DPLUnitTest01::Shape = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Shape", TestUnit);

const DPLEntity DPLUnitTest01::ShapeDirection = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Direction", Shape);

const DPLEntity DPLUnitTest01::ShapePath = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Path", Shape);

const DPLEntity DPLUnitTest01::Moving = DPLData::getMetaEntity(DPLUnitIdea::TypeType, "Moving", TestUnit);

const DPLEntity DPLUnitTest01::MovingVelocity = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Velocity", Moving);
const DPLEntity DPLUnitTest01::MovingAcceleration = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Acceleration", Moving);

const DPLEntity DPLUnitTest01::AgentHelloWorldSimple = DPLData::getMetaEntity(DPLUnitIdea::TypeAgent, "HelloWorldSimple", TestUnit);
