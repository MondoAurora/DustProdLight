#ifndef TEXT_H_
#define TEXT_H_

#include <dpl.h>

#include <_dplgen_module_dpl_stl.h>
#include <_dplgen_module_text.h>

class CharMatcher: public DPLNarrativeLogic {
public:
	virtual ~CharMatcher() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class TokenMatcher: public DPLNarrativeLogic {
public:
	virtual ~TokenMatcher() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class NumberSci: public DPLNarrativeLogic {
public:
	virtual ~NumberSci() {
	}
	virtual DPLProcessResult dplActionExecute();
};

class NumberRad: public DPLNarrativeLogic {
public:
	virtual ~NumberRad() {
	}
	virtual DPLProcessResult dplActionExecute();
};

#endif /* TEXT_H_ */
