#include "module.h"

#include "../dpl_impl_meta.h"
#include "data.h"

using namespace DPLImplMeta;

class DustProdLightDataModule: public DPLModule {
public:
	virtual ~DustProdLightDataModule() {

	}

	virtual void init() const {
		DustProdLightStore::init();
	}

	virtual void release() const {
		DustProdLightStore::release();
	}
};

const DPLModule* DPLModuleData = new DustProdLightDataModule();

