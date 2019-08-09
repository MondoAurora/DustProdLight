#include <iostream>

#include "dustprodlight.h"

DustProdLight::DustProdLight()
{
}

DustProdLight::~DustProdLight()
{
}


void DustProdLight::init() {
    Dust::initKernel(new DustProdLight);
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


//DustKey DustProdLight::getKeyImpl(const char* metaId) {
//    MapIdToKey::iterator it = mapIdToKey.find(metaId);

//    DustKey k = mapIdToKey[metaId];

//    if (it == mapIdToKey.end()) {
//        mapKeyToId.insert(MapKeyToId::value_type(k, metaId));
//    }

//    return k;
//}
