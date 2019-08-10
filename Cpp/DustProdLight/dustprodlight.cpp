#include <iostream>
#include <set>


#include "dustprodlight.h"


using namespace std;

DustProdLight::DustProdLight()
{
}

DustProdLight::~DustProdLight()
{
}


void DustProdLight::init() {
    Dust::initKernel(&self);
}

set<DustChangeListener*> changeListeners;

void DustProdLight::registerChangeListener(DustChangeListener *pListener)
{
    changeListeners.insert(pListener);
}


void DustProdLight::optPush() {
}


void DustProdLight::optPull() {
}


void DustProdLight::optNotify() {
    set<DustChangeListener*>::iterator it;
    for (it = changeListeners.begin(); it != changeListeners.end(); ++it)
    {
        DustChangeListener *l = *it;
        l->processChange();
    }
}

using namespace std;

void DustProdLight::accessImpl(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var) {
    DPLEntity e = ctx;

    switch (cmd) {
    case read:
        ctx.get(member, var);
        break;
    case write:
        ctx.set(member, var);
        break;
    default:
        break;
    }
}

void DustProdLight::dumpImpl() {
    cout << ctx;
}

DustProdLight DustProdLight::self;


//DustKey DustProdLight::getKeyImpl(const char* metaId) {
//    MapIdToKey::iterator it = mapIdToKey.find(metaId);

//    DustKey k = mapIdToKey[metaId];

//    if (it == mapIdToKey.end()) {
//        mapKeyToId.insert(MapKeyToId::value_type(k, metaId));
//    }

//    return k;
//}
