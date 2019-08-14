#ifndef DUSTPRODLIGHT_H
#define DUSTPRODLIGHT_H

#include <dust.h>

#include <dpljsonqt.h>

#include <QCoreApplication>

#include "dustprodlight_global.h"
#include "dplentity.h"

using namespace std;

//typedef map<DustKey, string> MapKeyToId;
//typedef map<string, DustKey> MapIdToKey;

class DUSTPRODLIGHTSHARED_EXPORT DustProdLight : public Dust
{
    friend class DPLJSONProcKernelLoader;

public:
    static void init(const char* metaVersion);
    static void init(QCoreApplication &app, const char* metaVersion);

    static void registerChangeListener(DustChangeListener *pListener);

    static void optPush();
    static void optPull();
    static void optNotify();

protected:
    DustProdLight();
    ~DustProdLight();

    void accessImpl(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var);
    void dumpImpl();

    void loadMeta();

private:
    DPLEntity ctx;
    static DustProdLight self;

    const char* dustPath;
};

#endif // DUSTPRODLIGHT_H
