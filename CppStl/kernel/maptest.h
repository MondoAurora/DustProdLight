#ifndef MAPTEST_H_
#define MAPTEST_H_

#include "dpl/dpl_impl.h"

class MapMeta {
public:
	DPLType Vector = DPLStore::getType("Vector");

	DPLToken Vector_X = DPLStore::getToken(Vector, "X", fmd_valtype_double);
	DPLToken Vector_Y = DPLStore::getToken(Vector, "Y", fmd_valtype_double);

	DPLType ID = DPLStore::getType("Identified");

	DPLToken ID_Name = DPLStore::getToken(ID, "Name", fmd_valtype_string);
	DPLToken ID_id = DPLStore::getToken(ID, "id", fmd_valtype_int);

	DPLType Position = DPLStore::getType("Position");

	DPLType Shape = DPLStore::getType("Shape");

	DPLToken Shape_Direction = DPLStore::getToken(Shape, "Direction", fmd_valtype_ref_single);

	DPLToken Shape_Path = DPLStore::getToken(Shape, "Path", fmd_valtype_ref_array);

	DPLType Moving = DPLStore::getType("Moving");

	DPLToken Moving_Velocity = DPLStore::getToken(Shape, "Velocity", fmd_valtype_ref_single);
	DPLToken Moving_Acceleration = DPLStore::getToken(Shape, "Acceleration", fmd_valtype_ref_single);
};

#endif /*MAPTEST_H_*/
