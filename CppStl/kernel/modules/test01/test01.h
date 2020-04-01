#ifndef TEST_H_
#define TEST_H_

#include <dpl.h>

#include <_dplgen_module_test01.h>


class HelloWorldSimple: public DPLAction {
public:
	virtual ~HelloWorldSimple() {
	}
	virtual DPLProcessResult dplProcess();
};

#endif /* TEST_H_ */
