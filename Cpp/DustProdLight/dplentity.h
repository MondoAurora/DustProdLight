#ifndef DPLENTITY_H
#define DPLENTITY_H

#include "dustprodlight_global.h"

#include <map>
#include <string>

#include <dust.h>

class DPLEntity
{
private:
    std::map<DustKey, DustVariant> data;

    friend DUSTPRODLIGHTSHARED_EXPORT std::ostream& operator<<(std::ostream&, const DPLEntity&);

public:
    DPLEntity();
    ~DPLEntity();

    void set(DustKey key, const DustVariant &val);
    void get(DustKey key, DustVariant &val);
};

#endif // DPLENTITY_H
