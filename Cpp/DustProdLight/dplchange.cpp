#include "dplchange.h"



DPLChangeValue::DPLChangeValue(const DustVariant &varOld, const DustVariant &varNew)
    :varOrig(varOld), varCurrent(varNew) { }

DPLChangeValue::~DPLChangeValue() {}

void DPLChangeValue::update(const DustVariant &varNew){
    DustVariant::set(varCurrent, varNew);
};

DPLChangeRef::DPLChangeRef(DustKey key, DustRefCommand refCommand, DPLRef* pRef)
    :key(key), refCommand(refCommand), pRef(pRef) {}

DPLChangeRef::~DPLChangeRef() {
};


DPLChangeEntity::DPLChangeEntity(DPLEntity *pEntity)
    :pEntity(pEntity) {}

DPLChangeEntity::~DPLChangeEntity() {};

void DPLChangeEntity::chgValue(DustKey keyRef, const DustVariant &varOld, const DustVariant &varNew) {
    DPLChangeValue* pCV;

    if ( valChanges.contains(keyRef) ) {
        pCV = valChanges[keyRef];
        pCV->update(varNew);
    } else {
        valChanges[keyRef] = pCV = new DPLChangeValue(varOld, varNew);
    }
}

void DPLChangeEntity::chgRef(DustKey keyRef, DustRefCommand cmd, DPLRef *pRef) {

}


DPLChange::DPLChange() { }

DPLChange::~DPLChange()
{

}

void DPLChange::chgValue(DPLEntity *pTarget, DustKey keyRef, const DustVariant &varOld, const DustVariant &varNew) {
    DPLChangeEntity* pCE;

    if ( entityChanges.contains(pTarget) ) {
        pCE = entityChanges[pTarget];
    } else {
        entityChanges[pTarget] = pCE = new DPLChangeEntity(pTarget);
    }

    pCE->chgValue(keyRef, varOld, varNew);
}

void DPLChange::chgRef(DPLEntity *pTarget, DustKey keyRef, DustRefCommand cmd, DPLRef *pRef) {

}


DPLChangeHint::DPLChangeHint( DPLEntity *pEntity, DustKey key)
    :pEntity(pEntity), key(key) {}

DPLChangeTransmitter::~DPLChangeTransmitter() {}

DPLChangeReceiver::~DPLChangeReceiver() {}
