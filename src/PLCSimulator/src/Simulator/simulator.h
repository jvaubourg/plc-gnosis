#ifndef PLC_GUIHELPER_H
#define PLC_GUIHELPER_H

#include <QMap>

//TODO Rename stuff in this class so it's consistent
#include "../PLCTopologyLoader/plctopologyloader.h"

#include <ns3/plc-defs.h>
#include <ns3/plc-node.h>
#include <ns3/plc.h>

#include "../BodeWidget/bodewidget.h"
#include "../SimulatorMainWindow/simulatormainwindow.h"

class PLCSimulator
{
public:
    PLCSimulator();
    PLCSimulator(QString modelFileName);
    void collectTransferFunctions();
    void showMainWindow();
    int numberOfPlots();

    static void exportTransferData(QString fileName, Ptr<PLC_TransferBase> ctf);

    //void showTransferFunction(QString from, QString to);

    static QVector<QVector<double> > ctfToPlottable(Ptr<PLC_TransferVector> ctf);
private:
    void setupWidgets();

    PLCTopologyLoader * loader;
    SimulatorMainWindow* mainWindow;
};

#endif // PLC_GUIHELPER_H
