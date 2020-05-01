#ifndef TEXT_H_
#define TEXT_H_

#include <dpl.h>

#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_text.h>

class CharMatcher: public DPLNativeLogic {
public:
	virtual ~CharMatcher() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class TokenMatcher: public DPLNativeLogic {
public:
	virtual ~TokenMatcher() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class NumberSci: public DPLNativeLogic {
public:
	virtual ~NumberSci() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class NumberRad: public DPLNativeLogic {
public:
	virtual ~NumberRad() {
	}
	virtual DPLProcessResult dplActionExecute();
};

#endif /* TEXT_H_ */
