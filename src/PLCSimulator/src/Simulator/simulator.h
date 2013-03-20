#ifndef PLC_GUIHELPER_H
#define PLC_GUIHELPER_H

#include <QMap>

//TODO Rename stuff in this class so it's consistent
#include "../PLCTopologyLoader/plctopologyloader.h"

#include <ns3/plc-defs.h>
#include <ns3/plc-node.h>
#include <ns3/plc.h>

#include "../BodeWidget/bodewidget.h"

#include "../SimulatorMainWindow/bodewidgetwindow.h"
#include "../GraphWidget/graphwidget.h"

class PLCSimulator
{
public:
    PLCSimulator();
    PLCSimulator(QString modelFileName);
    void collectTransferFunctions();
    void showPlotWindow();
    void showBodeWindow();
    int numberOfPlots();

    void psdTest();

    static void exportTransferData(QString fileName, Ptr<PLC_TransferBase> ctf);
    void simulateSINRAtReceiver(QString rxName, QString txName, int packetLength);

    //void showTransferFunction(QString from, QString to);

    static QVector<QVector<double> > ctfToPlottable(Ptr<PLC_TransferVector> ctf);
    static QVector<QVector<double> > spectrumValueToPlottable(Ptr<SpectrumValue> val);
private:
    void setupWidgets();

    PLCTopologyLoader * loader;
    BodeWidgetWindow* bodeWindow;

    GraphWidget * graphWidget;

    QMap<QString, QMap<QString,Ptr<PLC_TransferBase> > > transferFunctions;
};

#endif // PLC_GUIHELPER_H
