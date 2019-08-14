#include <iostream>
#include <set>

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

