#ifndef DPLJSONQT_H
#define DPLJSONQT_H

#include <dust.h>

#include <QFile>
#include <QMap>
#include <QJsonDocument>

#include <dplentity.h>
#include <dplchange.h>

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
public:
    static bool processJsonFile(QFile &file, DPLJSONProc &proc);

    static bool loadEntities(QFile &file);
    static bool saveEntities(QFile &file);
    static bool saveChanges(QFile &file, const QVector<void*> &transactions);

    static QString EMPTY_STRING;
};

#endif // DPLJSONQT_H
