#ifndef DPL_IMPL_BOOT_H_
#define DPL_IMPL_BOOT_H_

#include <dpl.h>

enum MontruBootItem {
	DPL_MBI_STORE_SOURCE = DPL_LAST_CONST,

	DPL_MBI_UNIT_MODEL,
	DPL_MBI_UNIT_IDEA,
	DPL_MBI_UNIT_NATIVE,

	DPL_MBI_TYPE_MODEL_STORE,
	DPL_MBI_TYPE_NATIVE_MODULE,

	DPL_MBI_ATT_ENTITY_GLOBALID,

	DPL_MBI_REF_CONNECTED_OWNER,

	DPL_MBI_, // end of list, USED in init cycle!
};

#endif /* DPL_IMPL_BOOT_H_ */
