#ifndef TEST_H_
#define TEST_H_

#include <dpl.h>

#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_test01.h>


class HelloWorldSimple: public DPLNativeLogic {
	int count = 0;
	DPLEntity e = DPL_ENTITY_INVALID;

public:
	virtual ~HelloWorldSimple() {
	}
	virtual DPLProcessResult dplActionExecute();
};

#endif /* TEST_H_ */
