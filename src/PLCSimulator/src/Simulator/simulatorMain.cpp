#include "simulator.h"
#include <QApplication>
#include "../PLCSimulatorSettingsWidget/plcsimulatorsettingswidget.h"
#include <QObject>

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    PLCSimulator simulator;
    //PLCSimulator simulator("./diagrams/ScenarioB.dgm");

    PLCSimulatorSettingsWidget* simSettings = new PLCSimulatorSettingsWidget(QString(""), simulator.getLoader()->getReceiverNames(), simulator.getLoader()->getTransmitterNames());

    QObject::connect(simSettings, SIGNAL(collectGivenTransferFunctions(QStringList,QStringList)), &simulator, SLOT(collectTransferFunctions(QStringList,QStringList)));
    QObject::connect(simSettings, SIGNAL(doNoiseSim(bool,bool,QString,QString)), &simulator, SLOT(simulateSINRAtReceiver(bool,bool,QString,QString)));

    simSettings->show();

    //simulator.collectTransferFunctions();
    ///simulator.simulateSINRAtReceiver("N1", "N2", 100000);

    //simulator.psdTest();

    //simulator.showBodeWindow();
    //simulator.showPlotWindow();

    //qDebug() << "Simulator process finished. Showing Plots.";
    app.exec();
    return 0;
}
