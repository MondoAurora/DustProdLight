#ifndef DPLCHANGE_H
#define DPLCHANGE_H

#include <dplentity.h>


class DPLChangeValue
{
private:
    DustVariant origValue;

public:
    DPLChangeValue();
    ~DPLChangeValue();
};

class DPLChangeRef
{
private:
    DustKey key;
    DustRefCommand refCommand;
    DPLRef origRef;

public:
    DPLChangeRef();
    ~DPLChangeRef();
};

class DPLChangeEntity
{
private:
    QMap<DustKey, DPLChangeValue> valChanges;
    QVector<DPLChangeRef> refChanges;

public:
    DPLChangeEntity();
    ~DPLChangeEntity();
};

class DPLChange
{
private:
    QMap<DPLEntity*, DPLChangeEntity> entityChanges;

public:
    DPLChange();
    ~DPLChange();

    void chgValue(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var);
    void chgRef(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx);
};

#endif // DPLCHANGE_H
