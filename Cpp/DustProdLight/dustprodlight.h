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

protected:
    DustProdLight();
    ~DustProdLight();

    void accessImpl(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var);
    void dumpImpl();
//    DustKey getKeyImpl(const char* metaId);

private:
    DPLEntity ctx;
//    MapIdToKey mapIdToKey;
//    MapKeyToId mapKeyToId;
};

#endif // DUSTPRODLIGHT_H
