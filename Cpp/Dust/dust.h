#ifndef DUST_H
#define DUST_H

#include "dust_global.h"

#include <string>

class DUSTSHARED_EXPORT DustKey
{
public:
    DustKey();

    bool operator<(const DustKey& k) const;
private:
    static int nextKeyId;

    int keyId;

    friend DUSTSHARED_EXPORT std::ostream& operator<<(std::ostream&, const DustKey&);
};

enum DustMetaType {
    dvtUnset, dvtBool, dvtLong, dvtDouble, dvtRaw
    , dvtRefSingle, dvtRefSet, dvtRefArray, dvtRefMap

    , dmtType
};

class DustKeyInfo
{
public:
    const char* name;
    DustKey key;
    DustMetaType valType;
    DustKey *pParentType;

    DustKeyInfo(const char* name);
};

class DUSTSHARED_EXPORT DustVariant
{
private:
    DustMetaType valType = dvtUnset;

    union {
        bool valBool;
        long valLong;
        double valDouble;
        void* valRaw;
    };

    size_t dataSize;

    void initType(DustMetaType vtNew);
    void verifyType(DustMetaType vt) const;

    friend DUSTSHARED_EXPORT std::ostream& operator<<(std::ostream&, const DustVariant&);

public:
    ~DustVariant();

    static void set(DustVariant &target, const DustVariant &src);

    DustMetaType getType() const;
    size_t getSize() const;

    void reset();

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

enum DustAccessCommand {
    read, write, visit
};

//class DUSTSHARED_EXPORT DustKernelLoader
//{
//};

class DUSTSHARED_EXPORT Dust
{
//    friend class DustKernelLoader;
private:
    static Dust* pDust;

    static DustKeyInfo* getKeyInfo(const char* metaId);

protected:
    Dust();
    virtual ~Dust();

    virtual void accessImpl(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var) = 0;
    virtual void dumpImpl() = 0;

    static void initKernel(Dust* pKernel);

    static void initKey(const char* metaId, const char *valTypeName, const char *parentTypeName);
    static DustMetaType getKeyType(const char *metaId);

public:
    static void access(DustAccessCommand cmd, DustKey entity, DustKey member, DustVariant &var);
    static DustKey getKey(const char* metaId);

    static void dump();
};

#endif // DUST_H
