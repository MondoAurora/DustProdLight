#include "dust.h"
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

typedef map<DustKey, string> MapKeyToId;
typedef map<string, DustKey> MapIdToKey;



Dust::Dust()
{
}

Dust::~Dust()
{
}

void Dust::initKernel(Dust *pKernel) {
    if ( pDust ) {
        exit(1);
    }

    pDust = pKernel;
}

void Dust::dump() {
    pDust->dumpImpl();
}

void Dust::access(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var) {
    pDust->accessImpl(cmd, entity, member, var);
}

static MapIdToKey mapIdToKey;
static MapKeyToId mapKeyToId;

DustKey Dust::getKey(const char* metaId) {
    MapIdToKey::iterator it = mapIdToKey.find(metaId);

    DustKey k = mapIdToKey[metaId];

    if (it == mapIdToKey.end()) {
        mapKeyToId.insert(MapKeyToId::value_type(k, metaId));
    }

    return k;
}

Dust* Dust::pDust;


std::ostream& operator<<(std::ostream& stream, const DustKey& key) {
    return stream << mapKeyToId[key];
}

int DustKey::nextKeyId = 0;

DustKey::DustKey() {
    keyId = nextKeyId++;
}

bool DustKey::operator<(const DustKey& k) const {
    return keyId < k.keyId;
}

DustChangeListener::~DustChangeListener()
{
}

void DustChangeListener::processChange()
{
}
