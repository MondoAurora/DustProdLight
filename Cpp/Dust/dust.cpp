#include "dust.h"
#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>

#include <cstdarg>

using namespace std;

static map<string, DustMetaType> mapTypeNames;

typedef map<DustKey, DustKeyInfo*> MapKeyToId;
typedef map<string, DustKeyInfo*> MapIdToKeyInfo;

DustKeyInfo::DustKeyInfo(const char* name)
{
    this->name = name;
    valType = dvtUnset;
}

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

    mapTypeNames["dvtBool"] = dvtBool;
    mapTypeNames["dvtLong"] = dvtLong;
    mapTypeNames["dvtDouble"] = dvtDouble;
    mapTypeNames["dvtRaw"] = dvtRaw;
    mapTypeNames["dvtRefSingle"] = dvtRefSingle;
    mapTypeNames["dvtRefSet"] = dvtRefSet;
    mapTypeNames["dvtRef]Array"] = dvtRefArray;
    mapTypeNames["dvtRefMap"] = dvtRefMap;
}

void Dust::dump() {
    pDust->dumpImpl();
}

void Dust::access(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var) {
    pDust->accessImpl(cmd, entity, member, var);
}

static MapIdToKeyInfo mapIdToKey;
static MapKeyToId mapKeyToId;

DustKeyInfo * Dust::getKeyInfo(const char* metaId) {
    MapIdToKeyInfo::iterator it = mapIdToKey.find(metaId);

    DustKeyInfo *pki;

    if (it == mapIdToKey.end()) {
        pki = new DustKeyInfo(metaId);

        mapIdToKey.insert(MapIdToKeyInfo::value_type(metaId, pki));
        mapKeyToId.insert(MapKeyToId::value_type(pki->key, pki));
    } else {
        pki = it->second;
    }

    return pki;
}

DustKey Dust::getKey(const char* metaId) {
    return getKeyInfo(metaId)->key;
}

void Dust::initKey(const char* metaId, const char *valTypeName, const char *parentTypeName) {
    DustMetaType vt = mapTypeNames[valTypeName];
    DustKeyInfo *pkiParent = getKeyInfo(parentTypeName);

    switch (pkiParent -> valType) {
    case dvtUnset:
        pkiParent->valType = dmtType;
        break;
    case dmtType:
        // that is OK
        break;
    default:
        exit(3);
    }

    DustKeyInfo *pki = getKeyInfo(metaId);

    pki->valType = vt;
    pki->pParentType = &pkiParent->key;

    cout << "initKey " << metaId << " valType " << valTypeName << " parentType " << parentTypeName << endl;
}

DustMetaType Dust::getKeyType(const char *metaId) {
    return getKeyInfo(metaId)->valType;
}

Dust* Dust::pDust;


std::ostream& operator<<(std::ostream& stream, const DustKey& key) {
    return stream << mapKeyToId[key]->name;
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
