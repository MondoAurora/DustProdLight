#ifndef DPLCHANGETRANSMITTERFILE_H
#define DPLCHANGETRANSMITTERFILE_H

#include <dplchange.h>

class DUSTPRODLIGHTSHARED_EXPORT DPLChangeTransmitterFile : public DPLChangeTransmitter
{
private:
    QString fileIn;
    QString fileOut;

public:
    DPLChangeTransmitterFile(QString dir, QString fileIn, QString fileOut);

    virtual bool transmit(QVector<void*> &transactions);
};

class DUSTPRODLIGHTSHARED_EXPORT DPLFileChannel {
private:
    DPLChangeTransmitterFile left;
    DPLChangeTransmitterFile right;

public:
    DPLFileChannel(QString dir);

    DPLChangeTransmitter *getLeft();
    DPLChangeTransmitter *getRight();
};

#endif // DPLCHANGETRANSMITTERFILE_H
