#ifndef MAPTEST_H_
#define MAPTEST_H_

#include "dpl/dpl_impl.h"

namespace MapMeta {
DPLEntity TestUnit = DPLMeta::getUnit("Test");
DPLEntity Test = DPLMeta::getType(TestUnit, "Test");

DPLEntity TestOdd = DPLMeta::getToken(Test, "Odd", DPL_TOKEN_VAL_BOOL);

DPLEntity Vector = DPLMeta::getType(TestUnit, "Vector");

DPLEntity VectorX = DPLMeta::getToken(Vector, "X", DPL_TOKEN_VAL_DOUBLE);
DPLEntity VectorY = DPLMeta::getToken(Vector, "Y", DPL_TOKEN_VAL_DOUBLE);

DPLEntity Id = DPLMeta::getType(TestUnit, "Identified");

DPLEntity IdName = DPLMeta::getToken(Id, "Name", DPL_TOKEN_VAL_STRING);
DPLEntity IdNum = DPLMeta::getToken(Id, "Num", DPL_TOKEN_VAL_INT);

DPLEntity Position = DPLMeta::getType(TestUnit, "Position");

DPLEntity Shape = DPLMeta::getType(TestUnit, "Shape");

DPLEntity ShapeDirection = DPLMeta::getToken(Shape, "Direction", DPL_TOKEN_REF_SINGLE);

DPLEntity ShapePath = DPLMeta::getToken(Shape, "Path", DPL_TOKEN_REF_ARR);

DPLEntity Moving = DPLMeta::getType(TestUnit, "Moving");

DPLEntity MovingVelocity = DPLMeta::getToken(Shape, "Velocity", DPL_TOKEN_REF_SINGLE);
DPLEntity MovingAcceleration = DPLMeta::getToken(Shape, "Acceleration", DPL_TOKEN_REF_SINGLE);
}

#endif /*MAPTEST_H_*/
