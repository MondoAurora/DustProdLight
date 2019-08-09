
#include "dplentity.h"

using namespace std;

DPLEntity::DPLEntity()
{

}

DPLEntity::~DPLEntity()
{

}

void DPLEntity::set(DustKey key, const DustVariant &val) {
    DustVariant::set(data[key], val);
}

void DPLEntity::get(DustKey key, DustVariant &val) {
    DustVariant::set(val, data[key]);
}

std::ostream& operator<<(std::ostream& stream, const DPLEntity& entity){
    map<DustKey, DustVariant>::const_iterator it;

    stream << "{" << std::endl;
    for ( it = entity.data.begin(); it != entity.data.end(); it++ )
    {
        stream << "  " << it->first << ": " << it->second << std::endl ;
    }

    return stream << "}" << std::endl;
}
