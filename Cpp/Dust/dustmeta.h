#ifndef DUSTMETA_H
#define DUSTMETA_H

#include <dust.h>

namespace DustMeta {


//class DUSTSHARED_EXPORT DustConsts
//{
//public:
//    static const DustKey PATH_END;
//};

class DUSTSHARED_EXPORT DustTypes
{
public:
    static const char VERSION[];

    static const DustKey Unit;

    static const DustKey RootKey;
    static const DustKey Identifier;
    static const DustKey Entity;
    static const DustKey Text;
};

class DUSTSHARED_EXPORT RootKey
{
public:
    static const DustKey Self;
    static const DustKey Message;
    static const DustKey Context;
};

class DUSTSHARED_EXPORT Identifier
{
public:
    static const DustKey Local;
    static const DustKey Global;
};

class DUSTSHARED_EXPORT Entity
{
public:
    static const DustKey PrimaryType;
    static const DustKey Types;
};

class DUSTSHARED_EXPORT Text
{
public:
    static const DustKey Alias;
    static const DustKey Description;
};

}


#endif // DUSTMETA_H
