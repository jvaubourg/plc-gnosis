#include "simulator.h"
#include <QApplication>

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    PLCSimulator simulator;
    //PLCSimulator simulator("./diagrams/ScenarioB.dgm");
    simulator.collectTransferFunctions();

    if(simulator.numberOfPlots() > 0){
        simulator.showMainWindow();
        app.exec();
    }
    else {
        qDebug() << "No plottable transfer functions. See the /data/ directory for exported files";
    }

    qDebug() << "Simulator process finished";

    return 0;
}
