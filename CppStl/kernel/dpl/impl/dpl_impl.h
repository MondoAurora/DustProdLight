/*
 * dpl_impl.h
 *
 * DustProdLight STL version - all implementation class declaration
 *
 *  Created on: Feb 12, 2020
 *      Author: Lorand Kedves
 */

#ifndef DPL_IMPL_H_
#define DPL_IMPL_H_

#include "../dpl.h"
#include "../dplutils.h"

using namespace std;

enum MontruBootItem {
//	DPL_MBI_INVALID,

	DPL_MBI_STORE_SOURCE = DPL_LAST_CONST,

	DPL_MBI_UNIT_TOOLS,

	DPL_MBI_UNIT_MODEL,
	DPL_MBI_UNIT_IDEA,
	DPL_MBI_UNIT_NARRATIVE,
	DPL_MBI_UNIT_DIALOG,

	DPL_MBI_TYPE_MODEL_STORE,
	DPL_MBI_TYPE_MODEL_UNIT,
	DPL_MBI_TYPE_MODEL_ENTITY,

	DPL_MBI_TYPE_IDEA_TYPE,
	DPL_MBI_TYPE_IDEA_ATTRIBUTE,
	DPL_MBI_TYPE_IDEA_REFERENCE,

	DPL_MBI_TYPE_TOOLS_CONNECTED,

	DPL_MBI_ATT_ENTITY_GLOBALID,
	DPL_MBI_REF_CONNECTED_OWNER,
	DPL_MBI_REF_CONNECTED_EXTENDS,

	DPL_MBI_, // end of list, USED in init cycle!
};

class DustProdLightContext {
public:
	static DPLEntity optResolveContext(DPLEntity entity);
};

#endif /* DPL_IMPL_H_ */
