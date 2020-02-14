#include "dplchange.h"

#include <QMapIterator>


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

void DPLChange::toReceiver(DPLChangeReceiver *pReceiver) {
    pReceiver->begin();

    QMapIterator<DPLEntity*, DPLChangeEntity*> it(entityChanges);
    while ( it.hasNext() ) {
        it.next();
        DPLEntity *pE = it.key();
        pReceiver->beginEntity(pE);

        DPLChangeEntity *pCe = it.value();

        QMapIterator<DustKey, DPLChangeValue*> itVc(pCe->valChanges);
        while ( itVc.hasNext() ) {
            itVc.next();
            pReceiver->chgValue(itVc.key(), itVc.value()->varCurrent);
        }

        QVectorIterator<DPLChangeRef*> itRc(pCe->refChanges);
        while ( itRc.hasNext() ) {
            DPLChangeRef *pcr = itRc.next();
            pReceiver->chgRef(pcr->key, pcr->refCommand, pcr->pRef);
        }

        pReceiver->endEntity(pE);
    }

    pReceiver->end();
}


DPLChangeHint::DPLChangeHint( DPLEntity *pEntity, DustKey key)
    :pEntity(pEntity), key(key) {}

DPLChangeTransmitter::~DPLChangeTransmitter() {}

DPLChangeReceiver::~DPLChangeReceiver() {}
