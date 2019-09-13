#include "dplchangetransmitterfile.h"

#include <QFile>
#include <QDir>

#include <dpljsonqt.h>

DPLChangeTransmitterFile::DPLChangeTransmitterFile(QString dir, QString fileIn, QString fileOut)
{
    QDir fd(dir);

    if ( !fd.exists() ) {
        fd.mkpath(".");
    }

    this->fileIn = dir + "\\" + fileIn;
    this->fileOut = dir + "\\" + fileOut;
}

bool DPLChangeTransmitterFile::transmit(QVector<void*> &transactions) {
    bool ret = false;

    if ( 0 < transactions.size() ) {
        QFile fOut(fileOut);

        if ( fOut.exists() ) {
            return false;
        }

        if ( DPLJsonQt::saveChanges(fOut, transactions) ) {
            transactions.clear();
            ret = true;
        }
    }

    QFile fIn(fileIn);

    if ( fIn.exists() ) {
        DPLJsonQt::loadEntities(fIn);
        ret = true;
    }

    return ret;
}

DPLFileChannel::DPLFileChannel(QString dir)
    :left(dir, "RightToLeft.json", "LeftToRight.json"), right(dir, "LeftToRight.json", "RightToLeft.json")
{

}

DPLChangeTransmitter *DPLFileChannel::getLeft() {
    return &left;
}
DPLChangeTransmitter *DPLFileChannel::getRight() {
    return &right;
}
