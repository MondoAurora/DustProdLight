#ifndef MAPTEST_H_
#define MAPTEST_H_

#include "dpl/dpl_impl.h"

namespace MapMeta {
DPLType Test = DPLMeta::getType("Test");

DPLToken TestOdd = DPLMeta::getToken(Test, "Odd", DPL_TOKEN_VAL_BOOL);

DPLType Vector = DPLMeta::getType("Vector");

DPLToken VectorX = DPLMeta::getToken(Vector, "X", DPL_TOKEN_VAL_DOUBLE);
DPLToken VectorY = DPLMeta::getToken(Vector, "Y", DPL_TOKEN_VAL_DOUBLE);

DPLType Id = DPLMeta::getType("Identified");

DPLToken IdName = DPLMeta::getToken(Id, "Name", DPL_TOKEN_VAL_STRING);
DPLToken IdNum = DPLMeta::getToken(Id, "Num", DPL_TOKEN_VAL_INT);

DPLType Position = DPLMeta::getType("Position");

DPLType Shape = DPLMeta::getType("Shape");

DPLToken ShapeDirection = DPLMeta::getToken(Shape, "Direction", DPL_TOKEN_REF_SINGLE);

DPLToken ShapePath = DPLMeta::getToken(Shape, "Path", DPL_TOKEN_REF_ARR);

DPLType Moving = DPLMeta::getType("Moving");

DPLToken MovingVelocity = DPLMeta::getToken(Shape, "Velocity", DPL_TOKEN_REF_SINGLE);
DPLToken MovingAcceleration = DPLMeta::getToken(Shape, "Acceleration", DPL_TOKEN_REF_SINGLE);
}

#endif /*MAPTEST_H_*/
