#ifndef DUSTPRODLIGHT_H
#define DUSTPRODLIGHT_H

#include <dust.h>

#include <dpljsonqt.h>

#include <QCoreApplication>
#include <set>
#include <map>


#include "dustprodlight_global.h"
#include "dplentity.h"

using namespace std;

//typedef map<DustKey, string> MapKeyToId;
//typedef map<string, DustKey> MapIdToKey;

class DUSTPRODLIGHTSHARED_EXPORT DustProdLight : public Dust
{
    friend class DPLJSONProcKernelLoader;
    friend class DPLJSONProcCloudLoader;

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

    static DPLEntity* getEntity(const QString &globalId);

private:
    static DustProdLight self;

    const char* dustPath;
    DPLEntity ctx;

    set<DPLEntity*> allEntities;
    map<QString, DPLEntity*> globalEntities;

    set<DustChangeListener*> changeListeners;

};

#endif // DUSTPRODLIGHT_H
