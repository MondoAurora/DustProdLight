#ifndef DPL_IMPL_BOOT_H_
#define DPL_IMPL_BOOT_H_

#include <dpl.h>

enum MontruBootItem {
	DPL_MBI_STORE_SOURCE = DPL_LAST_CONST,

	DPL_MBI_UNIT_TOOLS,

	DPL_MBI_UNIT_MODEL,
	DPL_MBI_UNIT_IDEA,
	DPL_MBI_UNIT_NARRATIVE,
	DPL_MBI_UNIT_DIALOG,
	DPL_MBI_UNIT_DUST,

	DPL_MBI_TYPE_MODEL_STORE,
	DPL_MBI_TYPE_MODEL_UNIT,
	DPL_MBI_TYPE_MODEL_ENTITY,
	DPL_MBI_ATT_ENTITY_GLOBALID,

	DPL_MBI_TYPE_IDEA_TYPE,
	DPL_MBI_TYPE_IDEA_ATTRIBUTE,
	DPL_MBI_TYPE_IDEA_REFERENCE,
	DPL_MBI_TYPE_IDEA_TAG,
	DPL_MBI_TYPE_IDEA_SERVICE,
	DPL_MBI_TYPE_IDEA_COMMAND,

	DPL_MBI_TYPE_NARRATIVE_ACTION,

	DPL_MBI_TYPE_TOOLS_CONNECTED,

	DPL_MBI_REF_CONNECTED_OWNER,
	DPL_MBI_REF_CONNECTED_EXTENDS,

	DPL_MBI_, // end of list, USED in init cycle!
};

#endif /* DPL_IMPL_BOOT_H_ */
