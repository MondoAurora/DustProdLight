#include <iostream>

#include <QCommandLineParser>

#include "dustprodlight.h"


using namespace std;

DustProdLight::DustProdLight()
{
}

DustProdLight::~DustProdLight()
{
}


void DustProdLight::init(const char* metaVersion) {
    Dust::initKernel(&self);    

    self.dustPath = "Q:\\LightIPS\\LightIPS\\wdir";

    DPLJsonQt::loadKernelConfig(metaVersion, self.dustPath);
}

void DustProdLight::init(QCoreApplication &app, const char* metaVersion) {
    init(metaVersion);
}

void DustProdLight::registerChangeListener(DustChangeListener *pListener)
{
    self.changeListeners.insert(pListener);
}

DPLEntity* DustProdLight::getEntity(const QString &globalId) {
    DPLEntity* ret;

    if ( DPLJsonQt::EMPTY_STRING == globalId ) {
        ret = new DPLEntity();
    } else {
        ret = self.globalEntities[globalId];
    }
    self.allEntities.insert(ret);

    return ret;
}

void DustProdLight::optPush() {
}


void DustProdLight::optPull() {
}


void DustProdLight::optNotify() {
    set<DustChangeListener*>::iterator it;
    for (it = self.changeListeners.begin(); it != self.changeListeners.end(); ++it)
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

