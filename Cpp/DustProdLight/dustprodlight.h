#ifndef DUSTPRODLIGHT_H
#define DUSTPRODLIGHT_H

#include <dust.h>

#include <dpljsonqt.h>
#include <dplchange.h>

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
    friend class DPLJsonQt;
    friend class DPLJSONProcCloudLoader;
    friend class DPLEntity;

public:
    static void init(const QString &dustDir);
    static void init(const QString &dustDir, QCoreApplication &app);

    static void setTransmitter(DPLChangeTransmitter* t);

    static bool loadData(const QString &fileName);
    static bool loadData(QFile &inFile);

    static void registerChangeListener(DustChangeListener *pListener);

    static void selectEntityById(DustKey keyCtxTarget, const QString &globalId);


//    static void setDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val);
//    static double getDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val);

    static bool getString(DustKey keyCtxTarget, DustKey keyRef, QString &str);

protected:
    DustProdLight();
    ~DustProdLight();

    static DustKeyInfo* getKeyInfo(const QString metaId);

    virtual void accessValueImpl(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var);
    virtual void selectEntityImpl(DustKey keyCtxTarget, DustKey keyCtxSource, int count, va_list &args);
    virtual bool accessRefImpl(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx = -1);
    virtual bool nextRefImpl(DustKey keyCtxTarget);
    virtual bool commSignalImpl(DustCommSignal dcs);

    void dumpImpl();

    static DPLEntity* getEntity(const QString &globalId);

    static DPLChange* getCurrentTransaction();
    static bool processTransactions(DPLChangeReceiver *preceiver);
    static void varChangeLogger(const DustVariant &varOld, const DustVariant &varNew, const void *pHint);

private:
    static DustProdLight self;

    QString dustPath;
    QMap<DustKey, DPLEntitySel*> ctx;

    QSet<DPLEntity*> allEntities;
    QMap<QString, DPLEntity*> globalEntities;


    DPLChangeTransmitter *chgTransmitter = nullptr;
    DPLChange* currTransaction = nullptr;
    QVector<void*> transactions;

    QSet<DustChangeListener*> changeListeners;

    DPLEntitySel* getSel(DustKey key);
};

#endif // DUSTPRODLIGHT_H
