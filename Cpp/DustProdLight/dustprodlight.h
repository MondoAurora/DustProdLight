#ifndef DUSTPRODLIGHT_H
#define DUSTPRODLIGHT_H

#include <dust.h>

//#include <map>

//#include "lazymap.hpp"

#include "dustprodlight_global.h"
#include "dplentity.h"

using namespace std;

//typedef map<DustKey, string> MapKeyToId;
//typedef map<string, DustKey> MapIdToKey;

class DUSTPRODLIGHTSHARED_EXPORT DustProdLight : public Dust
{
public:
    static void init();

    static void registerChangeListener(DustChangeListener *pListener);

    static void optPush();
    static void optPull();
    static void optNotify();

protected:
    DustProdLight();
    ~DustProdLight();

    void accessImpl(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var);
    void dumpImpl();
//    DustKey getKeyImpl(const char* metaId);

private:
    DPLEntity ctx;
    static DustProdLight self;
//    MapIdToKey mapIdToKey;
//    MapKeyToId mapKeyToId;
};

#endif // DUSTPRODLIGHT_H
