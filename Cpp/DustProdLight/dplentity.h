#ifndef DPLENTITY_H
#define DPLENTITY_H

#include "dustprodlight_global.h"

#include <QSet>
#include <QSetIterator>

#include <QVector>
#include <QVectorIterator>

#include <QMap>

#include <dust.h>

class DPLRef {
protected:
    void *pEntity = nullptr;

    DPLRef(DustRefType refType, void *pEntity);


    bool setEntity(void *pE);
    bool dropRef();

public:
    const DustRefType refType;

    DPLRef(void *pEntity);
    virtual ~DPLRef();

    void* getEntity();
    virtual bool isMulti();

    virtual bool access(DustRefCommand cmd, void *pParam, DPLRef *& currRef, int optIdx = -1);

    static DPLRef *createRef(DustRefType type, void *pEntity);
};

class DPLRefSet : public DPLRef {
public:
    QSet<DPLRefSet*> * setRef;

    DPLRefSet(DPLRefSet *pOrig, void *pEntity);
    virtual ~DPLRefSet();

    virtual bool isMulti();
    virtual bool access(DustRefCommand cmd, void *pParam, DPLRef *& currRef, int optIdx = -1);
};

class DPLRefArr : public DPLRef {
public:
    QVector<DPLRefArr*> * vecRef;

    DPLRefArr(DPLRefArr *pOrig, void *pEntity);
    virtual ~DPLRefArr();

    virtual bool isMulti();
    virtual bool access(DustRefCommand cmd, void *pParam, DPLRef *& currRef, int optIdx = -1);
};

class DPLEntity
{
private:
    int refCounter;

    QMap<DustKey, DustVariant> data;
    QMap<DustKey, DPLRef*> refs;

    friend DUSTPRODLIGHTSHARED_EXPORT std::ostream& operator<<(std::ostream&, const DPLEntity&);

public:
    const bool global;

    DPLEntity(bool global);
    ~DPLEntity();

    void addRef();
    bool removeRef();

    DustVariant * accessVar(DustKey key);

    void set(DustKey key, const DustVariant &val);
    void get(DustKey key, DustVariant &val);

    DPLRef * getRef(DustKey key, DustRefType refType = drtUnknown, DPLEntity *pParam = nullptr);
    bool accessRefImpl(DustKey key, DustRefType refType, DustRefCommand cmd, DPLEntity *pParam, int optIdx = -1);
};


#endif // DPLENTITY_H
