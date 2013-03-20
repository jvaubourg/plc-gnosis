#include "simulator.h"
#include <QApplication>

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    PLCSimulator simulator;
    //PLCSimulator simulator("./diagrams/ScenarioB.dgm");

    simulator.collectTransferFunctions();
    simulator.simulateSINRAtReceiver("N1", "N2", 100000);

    //simulator.psdTest();

    simulator.showBodeWindow();
    simulator.showPlotWindow();

    qDebug() << "Simulator process finished. Showing Plots.";
    app.exec();
    return 0;
}
