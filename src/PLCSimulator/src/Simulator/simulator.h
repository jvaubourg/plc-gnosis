#ifndef PLC_GUIHELPER_H
#define PLC_GUIHELPER_H

#include <QMap>

//TODO Rename stuff in this class so it's consistent
#include "../PLCTopologyLoader/plctopologyloader.h"

#include <ns3/plc-defs.h>
#include <ns3/plc-node.h>
#include <ns3/plc.h>

class PLCSimulator
{
public:
    PLCSimulator();
    PLCSimulator(QString modelFileName);
    void showTransferFunctions();
    void exportTransferData(QString fileName);

    //void showTransferFunction(QString from, QString to);

    static QVector<QVector<double> > ctfToPlottable(Ptr<PLC_TransferVector> ctf);
private:
    PLCTopologyLoader * loader;
};

#endif // PLC_GUIHELPER_H
