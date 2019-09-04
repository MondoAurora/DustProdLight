#include <iostream>

#include <QCommandLineParser>

#include "dustprodlight.h"
#include <dustmeta.h>

using namespace std;

DustProdLight::DustProdLight()
{
}

DustProdLight::~DustProdLight()
{
}


void DustProdLight::init() {
    Dust::initKernel(&self);    

    self.dustPath = "Q:\\LightIPS\\LightIPS\\wdir";
}

DustKeyInfo* DustProdLight::getKeyInfo(const QString metaId) {
    return Dust::getKeyInfo(qPrintable(metaId));
}


//void DustProdLight::loadMeta(const char* metaVersion, const char* fileName) {
//    DPLJsonQt::loadKernelConfig(metaVersion, self.dustPath, fileName);
//}

bool DustProdLight::loadData(const char* fileName) {
    return DPLJsonQt::loadEntities(self.dustPath, fileName);
}

void DustProdLight::init(QCoreApplication &) {
    init();
}

void DustProdLight::registerChangeListener(DustChangeListener *pListener)
{
    self.changeListeners.insert(pListener);
}

DPLEntity* DustProdLight::getEntity(const QString &globalId) {
    DPLEntity* ret;
    bool global = DPLJsonQt::EMPTY_STRING != globalId;

    if ( global ) {
        ret = self.globalEntities[globalId];
        if ( ret ) {
            return ret;
        }
    }

    ret = new DPLEntity(global);
    self.allEntities.insert(ret);
    if ( global ) {
        self.globalEntities[globalId] = ret;
    }

    return ret;
}

DPLEntitySel* DustProdLight::getSel(DustKey key) {
    DPLEntitySel* ret = self.ctx[key];

    if ( !ret ) {
        self.ctx[key] = ret = new DPLEntitySel();
    }

    return ret;
}

void DustProdLight::setDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val) {
    DPLEntity *pe = self.getSel(keyCtxTarget)->getCurrent();

    if ( pe ) {
        DustVariant *pv = pe->accessVar(keyRef);
        if ( pv ) {
            pv->setDouble(val);
        }
    }
}

double DustProdLight::getDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val) {
    DPLEntity *pe = self.getSel(keyCtxTarget)->getCurrent();

    if ( pe ) {
        DustVariant *pv = pe->accessVar(keyRef);
        if ( pv && (pv->getType() == dvtDouble) ) {
            double d;
            pv->getDouble(d);
            return d;
        }
    }

    return val;
}

bool DustProdLight::getString(DustKey keyCtxTarget, DustKey keyRef, QString &qs) {
    qs.clear();

    DPLEntity *pe = self.getSel(keyCtxTarget)->getCurrent();
    if ( pe ) {
        DustVariant *pv = pe->accessVar(keyRef);
        if ( pv && (pv->getType() == dvtRaw) ) {
            size_t l = pv->getSize();
            char* str = new char[l];
            pv->getRaw(str, l);
            qs.append(str);
            delete[] str;

            return true;
        }
    }

    return false;
}


void DustProdLight::optPush() {
}


void DustProdLight::optPull() {
}


void DustProdLight::optNotify() {
    QSet<DustChangeListener*>::iterator it;
    for (it = self.changeListeners.begin(); it != self.changeListeners.end(); ++it)
    {
        DustChangeListener *l = *it;
        l->processChange();
    }
}

using namespace std;

void DustProdLight::selectEntityById(DustKey keyCtxTarget, const QString &globalId) {
    DPLEntity *pe = self.getEntity(globalId);
    self.getSel(keyCtxTarget)->init(pe);
}

void DustProdLight::accessValueImpl(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var)  {
    DPLEntity* e = getSel(keyCtxTarget)->getCurrent();

    switch (cmd) {
    case read:
        e->get(keyRef, var);
        break;
    case write:
        e->set(keyRef, var);
        break;
    }
}

void DustProdLight::selectEntityImpl(DustKey keyCtxTarget, DustKey keyCtxSource, int count, va_list &args) {
    DPLEntitySel *ps = getSel(keyCtxSource);
    DPLEntity* pE = ps->getCurrent();

    DPLEntitySel *pt = getSel(keyCtxTarget);
    pt->init(pE, count, args);
}

bool DustProdLight::accessRefImpl(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx) {
    return false;
}

bool DustProdLight::nextRefImpl(DustKey keyCtxTarget)  {
    return getSel(keyCtxTarget)->next();
}


//DPLEntitySel::DPLEntitySel(DPLEntity *pE, DustKey path...)
//{
//    init(pE, path);
//}

DPLEntitySel::DPLEntitySel()
    :pCurrent(nullptr), pRef(nullptr)
{}

DPLEntitySel::~DPLEntitySel()
{
    if ( itVec ) {
        delete itVec;
    }
}

DPLEntity* DPLEntitySel::getCurrent() {
    return pCurrent;
}

bool DPLEntitySel::stepInto(DustKey refKey) {
    pRef = pCurrent->getRef(refKey);
    pCurrent = nullptr;

    if ( pRef ) {
        if ( pRef->isMulti() ) {
            switch ( pRef->refType ) {
            case drtSet:
                if ( itSet ) {
                    delete itSet;
                }
                itSet = new QSetIterator<DPLRefSet*>(*((DPLRefSet*)pRef)->setRef);
//                pCurrent = (DPLEntity*) itSet->next()->getEntity();
                break;
            case drtArray:
                if ( itVec ) {
                    delete itVec;
                }
                itVec = new QVectorIterator<DPLRefArr*>(*((DPLRefArr*)pRef)->vecRef);
//                pCurrent = (DPLEntity*) itVec->next()->getEntity();
                break;
            default:
                break;
            }
        } else {
            pCurrent = (DPLEntity*) pRef->getEntity();
        }
    }

    return (nullptr != pCurrent);
}

void DPLEntitySel::init(DPLEntity *pE) {
    pCurrent = pE;
    pRef = nullptr;
}

bool DPLEntitySel::init(DPLEntity *pE, int count, va_list &args) {
    init(pE);
    DustKey* p = va_arg(args, DustKey*);

    for (  int i = 0; i < count; ++i ) {
        if ( !stepInto(*p) ) {
            break;
        }
        p = va_arg(args, DustKey*);
    }

    return nullptr != pCurrent;
}

bool DPLEntitySel::next() {
    pCurrent = nullptr;

    if (!pRef || !pRef->isMulti() ) {
        return false;
    } else {
        switch ( pRef->refType ) {
        case drtSet:
            if ( itSet->hasNext() ) {
                pCurrent = (DPLEntity*) itSet->next()->getEntity();
            } else {
                pCurrent = nullptr;
            }
            break;
        case drtArray:
            if ( itVec->hasNext() ) {
                pCurrent = (DPLEntity*) itVec->next()->getEntity();
            } else {
                pCurrent = nullptr;
            }
            break;
        default:
            break;
        }

        return pCurrent != nullptr;
    }
}



void DustProdLight::dumpImpl() {
//    cout << ctx;
}

DustProdLight DustProdLight::self;

