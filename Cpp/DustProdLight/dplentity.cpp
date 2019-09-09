#include <stdarg.h>

#include "dplentity.h"
#include "dustmeta.h"

#include <QMapIterator>

//using namespace std;

DPLEntity::DPLEntity(bool global)
    :global(global)
{

}

DPLEntity::~DPLEntity()
{

}

void DPLEntity::addRef() {
    ++refCounter;
}

bool DPLEntity::removeRef() {
    --refCounter;

    if ( !global && (0 == refCounter) ) {
        delete this;
        return true;
    }

    return false;
}

DustVariant* DPLEntity::accessVar(DustKey key) {
    return &data[key];
}

void DPLEntity::set(DustKey key, const DustVariant &val) {
    DustVariant::set(data[key], val);
}

void DPLEntity::get(DustKey key, DustVariant &val) {
    DustVariant::set(val, data[key]);
}

DPLRef * DPLEntity::getRef(DustKey key, DustRefType refType, DPLEntity *pParam)
{
    DPLRef *pr = nullptr;

    if ( refs.find( key ) == refs.end() ) {
        if ( pParam && (drtUnset != refType) ) {
            pr = DPLRef::createRef(refType, pParam);
            refs.insert(key, pr);
        }
    } else {
        pr = refs[key];
    }

    return pr;
}

bool DPLEntity::accessRefImpl(DustKey key, DustRefType refType, DustRefCommand cmd, DPLEntity *pParam, int optIdx)
{
    DPLRef *pr = nullptr;

    if ( refs.find( key ) == refs.end() ) {
        if ( cmd == DustRefAdd ) {
            pr = DPLRef::createRef(refType, pParam);
            refs.insert(key, pr);
        }
        return true;
    } else {
        DPLRef *validRef = pr = refs[key];

       bool ret = pr->access(cmd, pParam, validRef, optIdx);

       if ( !validRef ) {
           refs.remove(key);
       } else if ( pr != validRef ) {
           refs[key] = validRef;
       }

       return ret;
    }
}


DPLRef * DPLRef::createRef(DustRefType refType, void *pEntity) {
    switch (refType ) {
    case drtSingle:
        return new DPLRef(pEntity);
    case drtSet:
        return new DPLRefSet(nullptr, pEntity);
    case drtArray:
        return new DPLRefArr(nullptr, pEntity);
    default:
        return nullptr;
    }
}

DPLRef::DPLRef(DustRefType refType, void *pEntity)
    :refType(refType)
{
    setEntity(pEntity);
}

DPLRef::DPLRef(void *pEntity)
    :refType(drtSingle)
{
    setEntity(pEntity);
}

DPLRef::~DPLRef() {
    dropRef();
}

bool DPLRef::isMulti() {
    return false;
}

void* DPLRef::getEntity() {
    return pEntity;
}


bool DPLRef::setEntity(void *pE) {
    if ( pEntity == pE ) {
        return false;
    }

    if ( pEntity ) {
        dropRef();
    }

    if ( pE ) {
        pEntity = pE;
        ((DPLEntity*)pEntity)->addRef();
    }

    return true;
}


bool DPLRef::dropRef() {
    return pEntity && ((DPLEntity*)pEntity)->removeRef();
}

bool DPLRef::access(DustRefCommand cmd, void *pParam, DPLRef *& currRef, int)
{
    switch ( cmd ) {
    case DustRefClear:
    case DustRefRemove:
        if ( pEntity ) {
            dropRef();
            currRef = nullptr;
            return true;
        }
        break;
    case DustRefAdd:
        return setEntity(pParam);
    }

    return false;
}

DPLRefSet::DPLRefSet(DPLRefSet *pOrig, void *pEntity)
    :DPLRef (drtSet, pEntity), setRef(nullptr)
{
    if ( pOrig ) {
        if ( pOrig->setRef ) {
            setRef = pOrig->setRef;
        } else {
            setRef = pOrig->setRef = new QSet<DPLRefSet*>;
            setRef->insert(pOrig);
        }
        setRef->insert(this);
    }
}

DPLRefSet::~DPLRefSet(){
    if ( setRef ) {
        setRef->remove(this);

        if ( 0 == setRef->size() ) {
            delete setRef;
            setRef = nullptr;
        }
    }
}

bool DPLRefSet::isMulti() {
    return (nullptr != setRef);
}

bool DPLRefSet::access(DustRefCommand cmd, void *pParam, DPLRef *& currRef, int){
    QMutableSetIterator<DPLRefSet*> qit(*setRef);

    switch ( cmd ) {
    case DustRefClear:
        for ( ; qit.hasNext(); ) {
            delete qit.next();
        }
        currRef = nullptr;
        return true;
    case DustRefRemove:
        for ( ; qit.hasNext(); ) {
            DPLRefSet* pRef = qit.next();
            if ( pParam == pRef->pEntity ) {
                if ( this == pRef ) {
                    currRef = *setRef->begin();
                }
                delete pRef;
                qit.remove();
                return true;
            }
        }
        return false;
    case DustRefAdd:
        for ( ; qit.hasNext(); ) {
            DPLRefSet* pRef = qit.next();
            if ( pParam == pRef->pEntity ) {
                return false;
            }
        }

        new DPLRefSet(this, pParam);

        return true;
    }

    return false;
};

DPLRefArr::DPLRefArr(DPLRefArr *pOrig, void *pEntity)
    :DPLRef (drtArray, pEntity), vecRef(nullptr)
{
    if ( pOrig ) {
        if ( pOrig->vecRef ) {
            vecRef = pOrig->vecRef;
        } else {
            vecRef = pOrig->vecRef = new QVector<DPLRefArr*>;
            vecRef->push_back(pOrig);
        }
        vecRef->push_back(this);
    }
}

DPLRefArr::~DPLRefArr() {
    if ( vecRef ) {
        vecRef->removeAll(this);

        if ( 0 == vecRef->size() ) {
            delete vecRef;
            vecRef = nullptr;
        }
    }
}

bool DPLRefArr::isMulti() {
    return (nullptr != vecRef);
}

bool DPLRefArr::access(DustRefCommand cmd, void *pParam, DPLRef *& currRef, int optIdx){
    if ( DustRefAdd == cmd ) {
        new DPLRefArr(this, pParam);
        return true;
    }

    if ( !vecRef ) {
        switch ( cmd ) {
        case DustRefAdd:
            // already handled above
            return false;
        case DustRefClear:
            currRef = nullptr;
            return setEntity(nullptr);
        case DustRefRemove:
            if ( (0 == optIdx ) || (pEntity == pParam) ){
                 delete this;
                 return true;
            }
            return false;
        }
    } else {
        QMutableVectorIterator<DPLRefArr*> qit(*vecRef);

        switch ( cmd ) {
        case DustRefAdd:
            // already handled above
            return false;
        case DustRefClear:
            for ( ; qit.hasNext(); ) {
                delete qit.next();
            }
            currRef = nullptr;
            return true;
        case DustRefRemove:
            if ( -1 == optIdx ) {
                for ( ; qit.hasNext(); ) {
                    ++optIdx;
                    DPLRefArr* pRef = qit.next();
                    if ( pParam == pRef->pEntity ) {
                        break;
                    }
                }
                if ( !qit.hasNext() ) {
                    return false;
                }
            }

            if ( (-1 != optIdx) && (optIdx < vecRef->size() ) ) {
                DPLRefArr* pRef = vecRef->at(optIdx);
                vecRef->removeAt(optIdx);
                if ( this == pRef ) {
                    currRef = vecRef->at(0);
                }
                delete pRef;
                return true;
            }
            return false;
        }

        return false;
    }

    return true;
}



std::ostream& operator<<(std::ostream& stream, const DPLEntity& entity){
    QMapIterator<DustKey, DustVariant> it = entity.data;

    stream << "{" << std::endl;
    for ( ; it.hasNext(); )
    {
        it.next();
        stream << "  " << it.key() << ": " << it.value() << std::endl ;
    }

    return stream << "}" << std::endl;
}
