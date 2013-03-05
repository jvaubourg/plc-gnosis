#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QVariantMap>



//#include <qjson/parser.h>

#include "MainWindow/mainwindow.h"

#include "nodemodel.h"
#include "NodeConfiguration/nodeconfiguration.h"

#include "PLCGraphicsNodeItem/plcgraphicsnodeitem.h"
#include "nodemodel.h"

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    //NodeModel nodeModelTest;
    //NodeConfiguration cfg(&nodeModelTest);

    NodeModel * node = new NodeModel();
    QList<NoiseSourceModel *> * noises = node->noiseSources();
    QList<NetDeviceModel *> * ndevices = node->netDevices();

    noises->append(new NoiseSourceModel());
    ndevices->append(new NetDeviceModel());
    node->setHasOutlet(true);


    //cfg.show();

    app.exec();
    return 0;

}


