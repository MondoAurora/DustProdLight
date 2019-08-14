#include "dust.h"
#include <stdlib.h>
#include <string>
#include <memory>

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

DustVariant::~DustVariant() {
    reset();
}

void DustVariant::reset() {
    initType(dvtUnset);
}

DustMetaType DustVariant::getType() const {
    return valType;
}

size_t DustVariant::getSize() const {
    return dataSize;
}

void DustVariant::verifyType(DustMetaType vt) const {
    if ( valType != vt ) {
        exit(2);
    }
}

void DustVariant::initType(DustMetaType vtNew) {
    if ( DustMetaType::dvtRaw == valType ) {
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

void DustVariant::set(DustVariant &target, const DustVariant &src) {
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
