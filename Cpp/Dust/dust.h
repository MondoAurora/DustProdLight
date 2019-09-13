#ifndef DUST_H
#define DUST_H

#include "dust_global.h"

#include <string>

using namespace std;

enum DustKeyType {
    DKT_Type, DKT_Ref, DKT_Value
};

enum DustValueType {
    dvtUnset = 0, dvtBool = 1, dvtLong = 2, dvtDouble = 3, dvtRaw = 4
};

enum DustRefType {
    drtUnset = 0, drtSingle = -1, drtSet = -2, drtArray = -3//, drtMap = -4 no "map" for now

    , drtType = 100
};

class DUSTSHARED_EXPORT DustKey
{
public:
    DustKey();

    bool operator<(const DustKey& k) const;
    bool operator==(const DustKey& k) const;
    bool operator!=(const DustKey& k) const;
private:
    static int nextKeyId;

    int keyId;

    friend DUSTSHARED_EXPORT std::ostream& operator<<(std::ostream&, const DustKey&);
};

class DUSTSHARED_EXPORT DustMetaInfo
{
public:
    DustKey key;
    const string id;

    DustMetaInfo(const char* pcId);
};

class DUSTSHARED_EXPORT DustUnitInfo : public DustMetaInfo
{
public:
    const string version;
    DustUnitInfo(const char* name, const char* version);
};

class DUSTSHARED_EXPORT DustKeyInfo : public DustMetaInfo
{
public:
    DustUnitInfo *pUnit;
    DustKeyInfo *pType;

    DustKeyType keyType;
    union {
        DustValueType valType;
        DustRefType refType;
    };

    static string buildId(const string id, DustUnitInfo *pU, DustKeyInfo *pT);

    DustKeyInfo(const char* name, DustUnitInfo *pUnit);
    DustKeyInfo(const char* name, DustKeyInfo *pType, DustValueType valType);
    DustKeyInfo(const char* name, DustKeyInfo *pType, DustRefType refType);

private:
    void regInfo();
};

typedef bool (*dustDoubleEquals)(const double &d1, const double &d2);

class DustVariant;

typedef void (*dustVarChgListener)(const DustVariant &varOld, const DustVariant &varNew, const void *pHint);

class DUSTSHARED_EXPORT DustVariant
{
private:
    DustValueType valType = dvtUnset;

    union {
        bool valBool;
        long valLong;
        double valDouble;
        void* valRaw = nullptr;
    };

    size_t dataSize = 0;

    void initType(DustValueType vtNew);
    void verifyType(DustValueType vt) const;

    friend DUSTSHARED_EXPORT std::ostream& operator<<(std::ostream&, const DustVariant&);
    friend class Dust;

public:
    DustVariant();
    DustVariant(const DustVariant& var);

    DustVariant(const bool b);
    DustVariant(const long& l);
    DustVariant(const double& d);
    DustVariant(const char* s);
    DustVariant(const void* r, const size_t size);

    ~DustVariant();

    DustValueType getType() const;
    size_t getSize() const;

    void reset();

    bool operator == (const DustVariant &var);

    bool operator == (const bool b);
    bool operator == (const long& l);
    bool operator == (const double& d);
    bool operator == (const char* s);

    static void set(DustVariant &target, const DustVariant &src, dustVarChgListener listener = nullptr, const void *pHint = nullptr);

    void setBool(bool b);
    void setLong(long l);
    void setDouble(double d);
    void setRaw(const char* s);
    void setRaw(const void* r, const size_t size);

    void getBool(bool &b) const;
    void getLong(long &l) const;
    void getDouble(double &d) const;
    size_t getRaw(void* r, const size_t size) const;
};

class DUSTSHARED_EXPORT DustChangeListener
{
public:
    virtual ~DustChangeListener();
    virtual void processChange();
};

enum DustEntityCommand {
    decDelete = 0, decCreate = 1, decChange = 2
};

enum DustValueCommand {
    read, write
};

enum DustRefCommand {
    DustRefClear, DustRefAdd, DustRefRemove
};

enum DustCommSignal {
    dcsOver, dcsOut
};

class DUSTSHARED_EXPORT Dust
{
private:
    static Dust* pDust;
    static dustDoubleEquals dblEquals;

protected:
    Dust();
    virtual ~Dust();

    virtual void accessValueImpl(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var) = 0;
    virtual void selectEntityImpl(DustKey keyCtxTarget, DustKey keyCtxSource, int count, va_list &args) = 0;
    virtual bool accessRefImpl(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx = -1) = 0;
    virtual bool nextRefImpl(DustKey keyCtxTarget) = 0;

    virtual bool commSignalImpl(DustCommSignal dcs) = 0;

    virtual void dumpImpl() = 0;

    static void initKernel(Dust* pKernel);
    static DustKeyInfo* getKeyInfo(const char* metaId);

public:
    static void setDoubleEquals(dustDoubleEquals de);
    static bool doubleEquals(const double &d1, const double &d2);

    static DustKey registerUnit(const char* unitId, const char* version);
    static DustKey registerKey(const char* id, const DustKey parent, DustKeyType = DKT_Type, const int contentType = 0);

    static void selectEntity(DustKey keyCtxTarget, DustKey keyCtxSource, int count, ...);

    static void accessValue(DustKey keyCtxTarget, DustKey keyRef, DustValueCommand cmd, DustVariant &var);
    static bool accessRef(DustKey keyCtxTarget, DustKey keyRef, DustRefCommand cmd, DustKey keyCtxParam, int optIdx = -1);
    static bool nextRef(DustKey keyCtxTarget);

    static void setDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val);
    static double getDouble(DustKey keyCtxTarget, DustKey keyRef, const double &val);

    static bool commSignal(DustCommSignal dcs);

    static void dump();
};

#endif // DUST_H
