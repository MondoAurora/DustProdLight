#include "dpljsonqt.h"

#include <QObject>
#include <QHash> // Faster lookup than in QMap
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

#include <dustprodlight.h>
#include <iostream>

static const QString LOADER_NAME = "DPLJsonQt";
static const long LOADER_VER = 1;

//static const char* JSONFILE_META = "dustMeta.json";

static const char* JSONKEY_DUSTLOADER = "dustJsonLoader";
static const char* JSONKEY_DUSTVER = "dustJsonVer";
static const char* JSONKEY_METAVER = "dustMetaVer";

//static const char* JSONKEY_ATTDEFS = "attdefs";

static const char* JSONKEY_NAME = "name";
//static const char* JSONKEY_TYPENAME = "typeName";
//static const char* JSONKEY_VALTYPE = "valType";

static const char* JSONKEY_ENTITIES = "entities";


DPLJSONProc::~DPLJSONProc() {};

DPLJSONProc::DPLJSONProc(){};

DPLJSONProc::DPLJSONProc(const char* ak, arrayProc ap)
{
    arrKey = ak;
    arrProc = ap;
}

bool DPLJSONProc::process(QJsonDocument &doc)
{
    QJsonArray jArr = doc.object().value(arrKey).toArray();

    for (int i = 0; i < jArr.count(); i++) {
        QJsonObject o(jArr.at(i).toObject());

        if ( !arrProc(o) ) {
            return false;
        }
    }

    return true;
};



//DPLJSONProcKernelLoader::~DPLJSONProcKernelLoader(){}

//DPLJSONProcKernelLoader::DPLJSONProcKernelLoader()
//    :DPLJSONProc (JSONKEY_ATTDEFS, DPLJSONProcKernelLoader::procKernelInfo)
//{}

//bool DPLJSONProcKernelLoader::procKernelInfo(QJsonObject &o) {
//    QString attParent = o.value(JSONKEY_TYPENAME).toString();
//    QString attName = o.value(JSONKEY_NAME).toString();
//    QString attType = o.value(JSONKEY_VALTYPE).toString();

//    attName.insert(0, ".");
//    attName.insert(0, attParent);

//    DustProdLight::initKey(qPrintable(attName), qPrintable(attType), qPrintable(attParent));

//    return true;
//}



DPLJSONProcCloudLoader::~DPLJSONProcCloudLoader() {};

DPLEntity* DPLJSONProcCloudLoader::getLocalEntity(QJsonObject &entities, const QString &locId) {
    DPLEntity *pe;

    if (localEntities.find( locId ) == localEntities.end() ) {        
        QJsonValue myEntity = entities.value(locId);
        QJsonObject myOb = myEntity.toObject();
        QJsonValue gid = myOb.value("DustCore.Identifier.Global");
        pe = DustProdLight::getEntity(gid.toString(DPLJsonQt::EMPTY_STRING));
        localEntities.insert(locId, pe);
    } else {
        pe = localEntities[locId];
    }

    return pe;
}

bool DPLJSONProcCloudLoader::process(QJsonDocument &doc)
{
    QJsonObject entities = doc.object().value(JSONKEY_ENTITIES).toObject();
    DustVariant var;
    DPLEntity *pe, *pp;
    DustKey k;

    for ( const auto& locId : entities.keys()  )
    {
        pe = getLocalEntity( entities, locId );

        QJsonObject data = entities.value(locId).toObject();

        for ( const auto& keyId : data.keys()  ) {
            QJsonValue jsonVal = data.value(keyId);
            DustKeyInfo *pki = DustProdLight::getKeyInfo(qPrintable(keyId));

            if ( !pki ) {
                std::cout << "Missing key " << qPrintable(keyId) << std::endl ;
                continue;
            }

            switch ( pki->keyType ) {
            case DKT_Value:
                var.reset();

                switch ( pki->valType ) {
                case dvtUnset:
                    // should not be here
                    exit(3);
                case dvtBool:
                    var.setBool(jsonVal.toBool());
                    break;
                case dvtLong:
                    var.setLong(jsonVal.toInt());
                    break;
                case dvtDouble:
                    var.setDouble(jsonVal.toDouble());
                    break;
                case dvtRaw:
                    var.setRaw(qPrintable(jsonVal.toString()));
                    break;
                }

                if ( dvtUnset != var.getType() ) {
                    pe->set(pki->key, var);
                }

                break;
            case DKT_Ref:
                switch ( pki->refType ) {
                case drtUnset:
                    // should not be here
                    exit(3);
                case drtSingle:
                    pp = getLocalEntity(entities, jsonVal.toString());
                    pe->accessRefImpl(pki->key, drtSingle, DustRefAdd, pp);
                    break;
                case drtSet:
                case drtArray:
                    QJsonArray ja = jsonVal.toArray();
                    int l = ja.size();
                    for ( int idx = 0; idx < l; ++idx ) {
                        QString ri = ja[idx].toString();
                        pp = getLocalEntity(entities, ri);
                        pe->accessRefImpl(pki->key, pki->refType, DustRefAdd, pp);
                    }
    //            case drtMap:
                    break;
                }
                break;
            }
        }
    }

    return true;
}


bool DPLJsonQt::processJsonFile(const char *jsonFile, DPLJSONProc &proc) {
    QFile file(jsonFile);
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QByteArray inputData = file.readAll();
        QJsonParseError jsonErr;
        QJsonDocument doc = QJsonDocument::fromJson(inputData, &jsonErr);
        if (jsonErr.error == QJsonParseError::NoError && doc.isObject() ) {
            // check type
            QJsonObject o = doc.object();

            if ( 0 != LOADER_NAME.compare(o.value(JSONKEY_DUSTLOADER).toString(""))) {
                return false;
            }
            if ( LOADER_VER != o.value(JSONKEY_DUSTVER).toInt(0) ) {
                return false;
            }
//            QString ms = o.value(JSONKEY_METAVER).toString("");
//            if ( (0 < ms.length()) && ( 0 != DPLJsonQt::metaVer.compare(ms)) ) {
//                return false;
//            }

            return proc.process(doc);
        }
        file.close();
    }

    return false;
};


//void DPLJsonQt::loadKernelConfig(const char* metaVer, const char *path, const char *fileName) {
//    DPLJsonQt::metaVer = metaVer;

//    DPLJSONProcKernelLoader kl;
//    QString kp = QString(path).append("\\").append(fileName);
//    processJsonFile(qPrintable(kp), kl);
//}

bool DPLJsonQt::loadEntities(const char *path, const char *fileName)
{
    DPLJSONProcCloudLoader cl;
    QString kp = QString(path).append("\\").append(fileName);
    return processJsonFile(qPrintable(kp), cl);
}

void DPLJsonQt::saveEntities(const char *, const char *)
{

}

//QString DPLJsonQt::metaVer;
QString DPLJsonQt::EMPTY_STRING = "@@@NOSTRING@@@";
