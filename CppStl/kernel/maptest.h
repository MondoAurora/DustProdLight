#ifndef MAPTEST_H_
#define MAPTEST_H_

#include "dpl/dpl_impl.h"

namespace MapMeta {
DPLType Test = DPL::getType("Test");

DPLToken TestOdd = DPL::getToken(Test, "Odd", DPL_TOKEN_VAL_BOOL);

DPLType Vector = DPL::getType("Vector");

DPLToken VectorX = DPL::getToken(Vector, "X", DPL_TOKEN_VAL_DOUBLE);
DPLToken VectorY = DPL::getToken(Vector, "Y", DPL_TOKEN_VAL_DOUBLE);

DPLType Id = DPL::getType("Identified");

DPLToken IdName = DPL::getToken(Id, "Name", DPL_TOKEN_VAL_STRING);
DPLToken IdNum = DPL::getToken(Id, "Num", DPL_TOKEN_VAL_INT);

DPLType Position = DPL::getType("Position");

DPLType Shape = DPL::getType("Shape");

DPLToken ShapeDirection = DPL::getToken(Shape, "Direction", DPL_TOKEN_REF_SINGLE);

DPLToken ShapePath = DPL::getToken(Shape, "Path", DPL_TOKEN_REF_ARR);

DPLType Moving = DPL::getType("Moving");

DPLToken MovingVelocity = DPL::getToken(Shape, "Velocity", DPL_TOKEN_REF_SINGLE);
DPLToken MovingAcceleration = DPL::getToken(Shape, "Acceleration", DPL_TOKEN_REF_SINGLE);
}

#endif /*MAPTEST_H_*/
