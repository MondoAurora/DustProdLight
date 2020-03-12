#ifndef MAPTEST_H_
#define MAPTEST_H_

#include "dpl/dpl.h"

namespace MapMeta {
DPLEntity TestUnit = DPLData::getMetaEntity(DPL_TOKEN_UNIT, "Test");
DPLEntity Test = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Test", TestUnit);

DPLEntity TestOdd = DPLData::getMetaEntity(DPL_TOKEN_VAL_BOOL, "Odd", Test);

DPLEntity Vector = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Vector", TestUnit);

DPLEntity VectorX = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "X", Vector);
DPLEntity VectorY = DPLData::getMetaEntity(DPL_TOKEN_VAL_DOUBLE, "Y", Vector);

DPLEntity Id = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Identified", TestUnit);

DPLEntity IdName = DPLData::getMetaEntity(DPL_TOKEN_VAL_STRING, "Name", Id);
DPLEntity IdNum = DPLData::getMetaEntity(DPL_TOKEN_VAL_INT, "Num", Id);

DPLEntity Position = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Position", TestUnit);

DPLEntity Shape = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Shape", TestUnit);

DPLEntity ShapeDirection = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Direction", Shape);

DPLEntity ShapePath = DPLData::getMetaEntity(DPL_TOKEN_REF_ARR, "Path", Shape);

DPLEntity Moving = DPLData::getMetaEntity(DPL_TOKEN_TYPE, "Moving", TestUnit);

DPLEntity MovingVelocity = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Velocity", Moving);
DPLEntity MovingAcceleration = DPLData::getMetaEntity(DPL_TOKEN_REF_SINGLE, "Acceleration", Moving);
}

#endif /*MAPTEST_H_*/
