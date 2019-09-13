#include "dust.h"
#include <stdlib.h>
#include <string>
#include <memory>


DustVariant::DustVariant() {}

DustVariant::DustVariant(const DustVariant& var) {
    switch (var.valType) {
    case dvtBool:
        setBool(var.valBool);
        break;
    case dvtLong:
        setLong(var.valLong);
        break;
    case dvtDouble:
        setDouble(var.valDouble);
        break;
    case dvtRaw:
        setRaw(var.valRaw, var.dataSize);
        break;
    case dvtUnset:
        break;
    }
}

DustVariant::DustVariant(const bool b) {
    setBool(b);
}
DustVariant::DustVariant(const long& l)  {
    setLong(l);
}
DustVariant::DustVariant(const double& d)  {
    setDouble(d);
}
DustVariant::DustVariant(const char* s) {
    setRaw(s);
}
DustVariant::DustVariant(const void* r, const size_t size) {
    setRaw(r, size);
}


DustVariant::~DustVariant() {
    reset();
}

void DustVariant::reset() {
    initType(dvtUnset);
}

DustValueType DustVariant::getType() const {
    return valType;
}

size_t DustVariant::getSize() const {
    return dataSize;
}

void DustVariant::verifyType(DustValueType vt) const {
    if ( valType != vt ) {
        exit(2);
    }
}

void DustVariant::initType(DustValueType vtNew) {
    if ( DustValueType::dvtRaw == valType ) {
        if ( nullptr != valRaw ) {
            free(valRaw);
            valRaw = nullptr;
        }
    }

    if ( valType != vtNew ) {
        valType = vtNew;

        switch (valType) {
        case dvtBool:
            dataSize = sizeof (bool);
            break;
        case dvtLong:
            dataSize = sizeof (long);
            break;
        case dvtDouble:
            dataSize = sizeof (double);
            break;
        default:
            dataSize = 0;
            valRaw = nullptr;
            break;
        }
    }
}

void DustVariant::set(DustVariant &target, const DustVariant &src, dustVarChgListener listener, const void *pHint) {
    if ( listener ) {
        if ( ! (target == src) ) {
            listener(target, src, pHint);
        }
    }

    switch (src.valType) {
    case dvtBool:
        target.setBool(src.valBool);
        break;
    case dvtLong:
        target.setLong(src.valLong);
        break;
    case dvtDouble:
        target.setDouble(src.valDouble);
        break;
    case dvtRaw:
        target.setRaw(src.valRaw, src.dataSize);
        break;
    case dvtUnset:
        target.reset();
        break;
    }
}

bool DustVariant::operator == (const DustVariant& var) {
    if ( valType == var.valType ) {
        switch (valType) {
        case dvtBool:
            return valBool == var.valBool;
        case dvtLong:
            return valLong == var.valLong;
        case dvtDouble:
            return Dust::doubleEquals(valDouble, var.valDouble);
        case dvtRaw:
            return (dataSize == var.dataSize) && !memcmp(valRaw, var.valRaw, dataSize);
        case dvtUnset:
            return true;
        }
    }

    return false;
}

bool DustVariant::operator == (const bool b) {
    return (dvtBool == valType) && (valBool == b);
}
bool DustVariant::operator == (const long& l) {
    return (dvtLong == valType) && (valLong == l);
}
bool DustVariant::operator == (const double& d) {
    return (dvtDouble == valType) && Dust::doubleEquals(valDouble, d);
}
bool DustVariant::operator == (const char* s) {
    return (dvtRaw == valType) && !strcmp(s, (char*) valRaw);
}



void DustVariant::setBool(bool b) {
    initType(dvtBool);
    valBool = b;
}

void DustVariant::setLong(long l) {
    initType(dvtLong);
    valLong = l;
}

void DustVariant::setDouble(double d) {
    initType(dvtDouble);
    valDouble = d;
}

void DustVariant::setRaw(const char* s) {
    if ( s ) {
        setRaw(s, strlen(s) + 1);
    }
}
void DustVariant::setRaw(const void* r, const size_t size) {
    initType(dvtRaw);

    if ( r && (0 < size) ) {
        dataSize = size;
        valRaw = malloc(dataSize);
        memcpy(valRaw, r, size);
    }
}



void DustVariant::getBool(bool &b) const {
    verifyType(dvtBool);
    b = valBool;
}

void DustVariant::getLong(long &l) const {
    verifyType(dvtLong);
    l = valLong;
}

void DustVariant::getDouble(double &d) const {
    verifyType(dvtDouble);
    d = valDouble;
}

size_t DustVariant::getRaw(void* r, const size_t size) const {
    verifyType(dvtRaw);

    size_t s = (size < dataSize) ? size : dataSize;

    memcpy(r, valRaw, s);

    return dataSize;
}



std::ostream& operator<<(std::ostream &strm, const DustVariant &var) {
    switch (var.valType) {
    case dvtBool:
        return strm << (var.valBool ? "true" : "false");
    case dvtLong:
        return strm << var.valLong;
    case dvtDouble:
        return strm << var.valDouble;
    case dvtRaw:
        strm.write((char*) var.valRaw, var.dataSize);
        return strm;
    default:
        return strm << "oops";
    }
}
