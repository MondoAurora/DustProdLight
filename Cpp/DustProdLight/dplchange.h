#ifndef DPLCHANGE_H
#define DPLCHANGE_H

#include <dplentity.h>


class DPLChangeTransmitter;
class DPLChangeReceiver;

class DPLChangeValue
{
    friend class DPLChange;

private:
    DustVariant varOrig;
    DustVariant varCurrent;

public:
    DPLChangeValue(const DustVariant &varOld, const DustVariant &varNew);
    ~DPLChangeValue();

    void update(const DustVariant &varNew);
};

class DPLChangeRef
{
    friend class DPLChange;

private:
    DustKey key;
    DustRefCommand refCommand;
    DPLRef* pRef;

public:
    DPLChangeRef(DustKey key, DustRefCommand refCommand, DPLRef* pRef);
    ~DPLChangeRef();
};

class DPLChangeEntity
{
    friend class DPLChange;

private:
    DustEntityCommand chgEntity;
    DPLEntity *pEntity;

    QMap<DustKey, DPLChangeValue*> valChanges;
    QVector<DPLChangeRef*> refChanges;

public:
    DPLChangeEntity(DPLEntity *pEntity);
    ~DPLChangeEntity();

    void chgValue(DustKey keyRef, const DustVariant &varOld, const DustVariant &varNew);
    void chgRef(DustKey keyRef, DustRefCommand cmd, DPLRef *pRef);
};

class DPLChange
{
private:
    QMap<DPLEntity*, DPLChangeEntity*> entityChanges;

public:
    DPLChange();
    ~DPLChange();

    void chgValue(DPLEntity *pTarget, DustKey keyRef, const DustVariant &varOld, const DustVariant &varNew);
    void chgRef(DPLEntity *pTarget, DustKey keyRef, DustRefCommand cmd, DPLRef *pRef);

    void toReceiver(DPLChangeReceiver *pReceiver);
};

class DPLChangeHint{
public:
    DPLEntity *pEntity;
    DustKey key;

    DPLChangeHint( DPLEntity *pEntity, DustKey key);
};

class DUSTPRODLIGHTSHARED_EXPORT DPLChangeTransmitter
{
public:
    virtual ~DPLChangeTransmitter();

    virtual bool transmit(QVector<void*> &transactions) = 0;
};

class DUSTPRODLIGHTSHARED_EXPORT DPLChangeReceiver
{
public:
    virtual ~DPLChangeReceiver();

    virtual void begin() = 0;
    virtual void beginEntity(void* pEntity) = 0;
    virtual void chgValue(DustKey keyRef, DustVariant &var) = 0;
    virtual void chgRef(DustKey keyRef, DustRefCommand cmd, void *pRef) = 0;
    virtual void endEntity(void* pEntity) = 0;
    virtual void end() = 0;
};

#endif // DPLCHANGE_H
