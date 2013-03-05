#include "nodeconfiguration.h"
#include <QHBoxLayout>

#include <QListWidget>
#include <QPushButton>
#include "netdevicedialog.h"
#include "noisesourcedialog.h"
#include "plcvaluestring.h"
#include "netdevicemodel.h"
#include "noisesourcemodel.h"

#include <QDebug>
#include <QCloseEvent>

NodeConfiguration::NodeConfiguration(NodeModel *node, QDialog *parent) :
    QDialog(parent)
{
    nodeModel = node;

    this->setWindowTitle(node->getName());
    //Setup the GUI Components/GroupBoxes etc.
    outletSettings = new QGroupBox("Outlet");
    outletSettings->setCheckable(true);
    outletSettings->setChecked(false);

    QVBoxLayout* outletSettingsLayout = new QVBoxLayout();
    outletImpedanceInput = new PLCDataInputWidget("Impedance");
    outletSettingsLayout->addWidget(outletImpedanceInput);
    outletSettings->setLayout(outletSettingsLayout);

    QGroupBox* netDevicesGroupBox = new QGroupBox("Net Devices");
    QVBoxLayout* netDevicesSettingsLayout = new QVBoxLayout();
    netDevicesView = new QListWidget();

    netDevicesSettingsLayout->addWidget(netDevicesView);
    netDevicesGroupBox->setLayout(netDevicesSettingsLayout);

    QGroupBox* noiseSourcesGroupBox = new QGroupBox("Noise Sources");
    QVBoxLayout* noiseSourcesSettingsLayout = new QVBoxLayout();
    noiseSourcesView = new QListWidget();

    noiseSourcesSettingsLayout->addWidget(noiseSourcesView);
    noiseSourcesGroupBox->setLayout(noiseSourcesSettingsLayout);

    QVBoxLayout * groupBoxesLayout = new QVBoxLayout();
    groupBoxesLayout->addWidget(outletSettings);
    groupBoxesLayout->addWidget(netDevicesGroupBox);
    groupBoxesLayout->addWidget(noiseSourcesGroupBox);

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    QPushButton * closeButton = new QPushButton("Close");
    buttonsLayout->addWidget(closeButton);
    groupBoxesLayout->addLayout(buttonsLayout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

    this->setLayout(groupBoxesLayout);

    populateFromModel();

    connect(netDevicesView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editNetDevices(QListWidgetItem*)));
    connect(noiseSourcesView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editNoiseSources(QListWidgetItem*)));

}

void NodeConfiguration::populateFromModel(){
    netDevicesView->clear();
    noiseSourcesView->clear();

    netDevicesView->addItem("<Add Net Device>");
    noiseSourcesView->addItem("<Add Noise Source>");

    outletSettings->setChecked(nodeModel->getHasOutlet());
    outletImpedanceInput->setValue(nodeModel->getOutletImpedance().getValue());

    for(int i = 0; i < nodeModel->netDevices()->length(); i++){
        netDevicesView->insertItem(i, nodeModel->netDevices()->at(i)->getName());
    }

    for(int i = 0; i <nodeModel->noiseSources()->length(); i++){
        noiseSourcesView->insertItem(i, nodeModel->noiseSources()->at(i)->getName());
    }
}

void NodeConfiguration::setModel(NodeModel *newModel){
    nodeModel = newModel;
    populateFromModel();
}

void NodeConfiguration::editNetDevices(QListWidgetItem *item){
    int currentRow = netDevicesView->currentRow();

    if(currentRow == netDevicesView->count() - 1){
        netDevicesView->insertItem(netDevicesView->count() - 1, "New Device");

        QString newNetDeviceName = nodeModel->getName() + QString(':')
                + QString("Dev") + QString::number(netDevicesView->count() - 2);

        nodeModel->netDevices()->append(new NetDeviceModel(newNetDeviceName));
        item = netDevicesView->item(currentRow);
    }

    QDialog * dialog = new NetDeviceDialog(nodeModel->netDevices()->at(currentRow), item, this);
    int result = dialog->exec();

    if(result == -1){
        nodeModel->netDevices()->removeAt(currentRow);
        delete(netDevicesView->takeItem(currentRow));
    }

    delete dialog;
}

void NodeConfiguration::editNoiseSources(QListWidgetItem *item){
    int currentRow = noiseSourcesView->currentRow();

    if(currentRow == noiseSourcesView->count() - 1){
       noiseSourcesView->insertItem(noiseSourcesView->count() - 1, "New Source");
       nodeModel->noiseSources()->append(new NoiseSourceModel());
       item = noiseSourcesView->item(currentRow);
    }

    QDialog * dialog = new NoiseSourceDialog(nodeModel->noiseSources()->at(currentRow), item, this);
    int result = dialog->exec();

    if(result == -1){
        nodeModel->noiseSources()->removeAt(currentRow);
        delete(noiseSourcesView->takeItem(currentRow));
    }

    delete dialog;
}

void NodeConfiguration::closeEvent(QCloseEvent * event){
    event->ignore();
    return;
}

void NodeConfiguration::saveAndClose(){
    if(outletSettings->isChecked()){
        if(outletImpedanceInput->isValid()){
            this->nodeModel->setHasOutlet(true);
            this->nodeModel->setOutletImpedance(outletImpedanceInput->getValue());
            done(0);
            return;
        }
    }
    else{
        this->nodeModel->setHasOutlet(false);
        done(0);
        return;
    }


}
