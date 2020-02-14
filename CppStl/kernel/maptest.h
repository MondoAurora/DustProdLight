#ifndef MAPTEST_H_
#define MAPTEST_H_

#include "dpl/dpl_impl.h"

namespace MapMeta {
	DPLType Vector = DPL::get_type("Vector");

	DPLToken Vector_X = DPL::get_token(Vector, "X", dpl_token_val_double);
	DPLToken Vector_Y = DPL::get_token(Vector, "Y", dpl_token_val_double);

	DPLType ID = DPL::get_type("Identified");

	DPLToken ID_Name = DPL::get_token(ID, "Name", dpl_token_val_string);
	DPLToken ID_id = DPL::get_token(ID, "id", dpl_token_val_int);

	DPLType Position = DPL::get_type("Position");

	DPLType Shape = DPL::get_type("Shape");

	DPLToken Shape_Direction = DPL::get_token(Shape, "Direction", dpl_token_ref_single);

	DPLToken Shape_Path = DPL::get_token(Shape, "Path", dpl_token_ref_array);

	DPLType Moving = DPL::get_type("Moving");

	DPLToken Moving_Velocity = DPL::get_token(Shape, "Velocity", dpl_token_ref_single);
	DPLToken Moving_Acceleration = DPL::get_token(Shape, "Acceleration", dpl_token_ref_single);
};

#endif /*MAPTEST_H_*/
