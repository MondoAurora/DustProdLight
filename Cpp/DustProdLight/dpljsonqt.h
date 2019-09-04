#ifndef DPLJSONQT_H
#define DPLJSONQT_H

#include <dust.h>

#include <QMap>
#include <QJsonDocument>

#include <dplentity.h>

typedef bool (*arrayProc)(QJsonObject &ob);

class DPLJSONProc {
private:
    arrayProc arrProc;
    const char* arrKey;

protected:
    DPLJSONProc();
public:
    DPLJSONProc(const char* arrayKey, arrayProc ap);
    virtual ~DPLJSONProc();

    virtual bool process(QJsonDocument &doc);
};

//class DPLJSONProcKernelLoader : public DPLJSONProc {
//public:
//    DPLJSONProcKernelLoader();
//    virtual ~DPLJSONProcKernelLoader();

//private:
//    static bool procKernelInfo(QJsonObject &o);
//};

class DPLJSONProcCloudLoader : public DPLJSONProc {
public:
    virtual ~DPLJSONProcCloudLoader();
    virtual bool process(QJsonDocument &doc);

private:
    QMap<QString, DustKey> units;
    QMap<QString, DPLEntity*> localEntities;

    DPLEntity *getLocalEntity(QJsonObject &entities, const QString &localId);
};


class DPLJsonQt //: public DustKernelLoader
{
private:
//    static QString metaVer;

public:
    static bool processJsonFile(const char *jsonFile, DPLJSONProc &proc);

//    static void loadKernelConfig(const char *metaVer, const char *dustPath, const char *fileName);

    static bool loadEntities(const char *path, const char *fileName);
    static void saveEntities(const char *path, const char *fileName);

    static QString EMPTY_STRING;
};

#endif // DPLJSONQT_H
