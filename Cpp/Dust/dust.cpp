#include "dust.h"
#include <stdlib.h>
#include <string>
#include <map>
#include <set>
#include <iostream>

#include <cstdarg>

using namespace std;

int DustKey::nextKeyId = 0;

DustKey::DustKey() {
    keyId = nextKeyId++;
}

bool DustKey::operator<(const DustKey& k) const {
    return keyId < k.keyId;
}

bool DustKey::operator==(const DustKey& k) const {
    return keyId == k.keyId;
}

bool DustKey::operator!=(const DustKey& k) const {
    return keyId != k.keyId;
}



typedef map<DustKey, DustKeyInfo*> MapKeyToId;
typedef map<string, DustKeyInfo*> MapIdToKeyInfo;
typedef map<DustKey, DustUnitInfo*> MapKeyToUnitInfo;

static MapKeyToUnitInfo mapUnits;
static MapIdToKeyInfo mapIdToKey;
static MapKeyToId mapKeyToId;


DustMetaInfo::DustMetaInfo(const char* name)
    :id(name)
{
};

DustUnitInfo::DustUnitInfo(const char* n, const char* v)
    :DustMetaInfo (n), version(v)
{
    mapUnits.insert(MapKeyToUnitInfo::value_type(key, this));
};

DustKeyInfo::DustKeyInfo(const char* n, DustUnitInfo *u)
    :DustMetaInfo (n), pUnit(u), pType(nullptr), keyType(DKT_Type)
{
    regInfo();
}

DustKeyInfo::DustKeyInfo(const char* n, DustKeyInfo *t, DustValueType vt)
    :DustMetaInfo (n), pUnit(t->pUnit), pType(t), keyType(DKT_Value), valType(vt)
{
    regInfo();
}

DustKeyInfo::DustKeyInfo(const char* n, DustKeyInfo *t, DustRefType rt)
    :DustMetaInfo (n), pUnit(t->pUnit), pType(t), keyType(DKT_Ref), refType(rt)
{
    regInfo();
}

string DustKeyInfo::buildId(const string id, DustUnitInfo *pU, DustKeyInfo *pT)
{
    return ( pT )
            ? pT->pUnit->id + "." + pT->id + "." + id
            :pU->id + "." + id;
}

void DustKeyInfo::regInfo()
{
    string longId = buildId(id, pUnit, pType);

    mapIdToKey.insert(MapIdToKeyInfo::value_type(longId, this));
    mapKeyToId.insert(MapKeyToId::value_type(key, this));
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
}

DustKey Dust::registerUnit(const char* unitId, const char* version) {
    DustUnitInfo *p = new DustUnitInfo(unitId, version);
    return p->key;
}

DustKey Dust::registerKey(const char* id, const DustKey parent, DustKeyType kt, const int contentType)
{
    DustUnitInfo *pU;
    DustKeyInfo *pT;

    if ( DKT_Type == kt ) {
        pU = mapUnits[parent];
        pT = nullptr;
    } else {
        pT = mapKeyToId[parent];
        pU = pT->pUnit;
    }
    string longId = DustKeyInfo::buildId(id, pU, pT);

    DustKeyInfo *p;

    if ( mapIdToKey.find(longId) == mapIdToKey.end() ) {
        switch ( kt ) {
        case DKT_Type:
            p = new DustKeyInfo(id, pU);
            break;
        case DKT_Value:
            p = new DustKeyInfo(id, pT, (DustValueType) contentType);
            break;
        case DKT_Ref:
            p = new DustKeyInfo(id, pT, (DustRefType) contentType);
            break;
        }
    } else {
        p = mapIdToKey[longId];
    }

    return p->key;
}

DustKeyInfo * Dust::getKeyInfo(const char* metaId) {
    return mapIdToKey[metaId];
}

void Dust::accessValue(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var) {
    pDust->accessValueImpl(keyCtxTarget, keyRef, cmd, var);
}
void Dust::selectEntity(DustKey keyCtxTarget, DustKey keyCtxSource, int count, ...) {
    va_list args;
    va_start(args, count);

    pDust->selectEntityImpl(keyCtxTarget, keyCtxSource, count, args);

    va_end(args);
}
bool Dust::accessRef(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx) {
    return pDust->accessRefImpl(keyCtxTarget, keyRef, cmd, keyCtxParam, optIdx);
}
bool Dust::nextRef(DustKey keyCtxTarget)  {
    return pDust->nextRefImpl(keyCtxTarget);
}


void Dust::dump() {
    pDust->dumpImpl();
}

Dust* Dust::pDust;


std::ostream& operator<<(std::ostream& stream, const DustKey& key) {
    return stream << mapKeyToId[key]->id;
}

DustChangeListener::~DustChangeListener()
{
}

void DustChangeListener::processChange()
{
}
