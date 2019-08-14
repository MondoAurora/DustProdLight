#ifndef DPLJSONQT_H
#define DPLJSONQT_H

#include <dust.h>

#include <map>

#include <QJsonDocument>

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

class DPLJSONProcKernelLoader : public DPLJSONProc {
public:
    DPLJSONProcKernelLoader();
    virtual ~DPLJSONProcKernelLoader();
//    virtual bool process(QJsonDocument &doc);

private:
    static bool procKernelInfo(QJsonObject &o);
};

class DPLJSONProcCloudLoader : public DPLJSONProc {
public:
    virtual ~DPLJSONProcCloudLoader();
    virtual bool process(QJsonDocument &doc);
};


class DPLJsonQt //: public DustKernelLoader
{
private:
    static QString metaVer;

public:
    static bool processJsonFile(const char *jsonFile, DPLJSONProc &proc);

    static void loadKernelConfig(const char *metaVer, const char *dustPath);

    static void loadEntities(const char *jsonFile);
    static void saveEntities(const char *jsonFile);
};

#endif // DPLJSONQT_H
