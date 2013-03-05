#include "simulator.h"
#include <QApplication>

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    //PLCSimulator simulator;
    PLCSimulator simulator("./diagrams/ScenarioB.dgm");
    simulator.showTransferFunctions();

    app.exec();
    return 0;
}
