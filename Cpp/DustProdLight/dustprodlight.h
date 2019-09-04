#ifndef DUSTPRODLIGHT_H
#define DUSTPRODLIGHT_H

#include <dust.h>

#include <dpljsonqt.h>

#include <QCoreApplication>
#include <QList>
#include <QSet>
#include <QMap>
#include <QString>

#include "dustprodlight_global.h"
#include "dplentity.h"

using namespace std;

class DPLEntitySel
{
    DPLEntity *pCurrent;
    DPLRef *pRef;

    union {
        QSetIterator<DPLRefSet*>* itSet;
        QVectorIterator<DPLRefArr*>* itVec = nullptr;
    };

public:
    DPLEntitySel();
    ~DPLEntitySel();

    bool stepInto(DustKey refKey);

    void init(DPLEntity *pE);
    bool init(DPLEntity *pE, int count, va_list &args);
    DPLEntity* getCurrent();
    bool next();
};


class DUSTPRODLIGHTSHARED_EXPORT DustProdLight : public Dust
{
    friend class DPLJSONProcKernelLoader;
    friend class DPLJSONProcCloudLoader;

public:
    static void init();
    static void init(QCoreApplication &app);

    static void loadMeta(const char* metaVersion, const char* fileName);
    static bool loadData(const char* fileName);

    static void registerChangeListener(DustChangeListener *pListener);

    static void selectEntityById(DustKey keyCtxTarget, const QString &globalId);


    static void setDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val);
    static double getDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val);

    static bool getString(DustKey keyCtxTarget, DustKey keyRef, QString &str);

    static void optPush();
    static void optPull();
    static void optNotify();

protected:
    DustProdLight();
    ~DustProdLight();

    static DustKeyInfo* getKeyInfo(const QString metaId);

    virtual void accessValueImpl(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var);
    virtual void selectEntityImpl(DustKey keyCtxTarget, DustKey keyCtxSource, int count, va_list &args);
    virtual bool accessRefImpl(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx = -1);
    virtual bool nextRefImpl(DustKey keyCtxTarget);
    void dumpImpl();

//    void loadMeta();

    static DPLEntity* getEntity(const QString &globalId);

private:
    static DustProdLight self;

    const char* dustPath;
    QMap<DustKey, DPLEntitySel*> ctx;

    QSet<DPLEntity*> allEntities;
    QMap<QString, DPLEntity*> globalEntities;

    QSet<DustChangeListener*> changeListeners;

    DPLEntitySel* getSel(DustKey key);
};

#endif // DUSTPRODLIGHT_H
