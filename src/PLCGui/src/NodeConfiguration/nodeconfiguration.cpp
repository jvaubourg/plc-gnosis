#include "nodeconfiguration.h"
#include <QHBoxLayout>

#include <QListWidget>
#include <QPushButton>
#include "netdeviceeditor.h"
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

    QGroupBox* netDeviceGroupBox = new QGroupBox("Net Device");
    netDeviceGroupBox->setCheckable(true);

    QWidget* netDeviceEditor = new NetDeviceEditor(node->getNetDevice());
    netDevicesView = new QListWidget();

    QVBoxLayout* netDevConfigLayout = new QVBoxLayout();
    netDevConfigLayout->addWidget(netDeviceEditor);

    netDeviceGroupBox->setLayout(netDevConfigLayout);

    QGroupBox* noiseSourceGroupBox = new QGroupBox("Noise Source");
    noiseSourceGroupBox->setCheckable(true);

    QVBoxLayout* noiseSrcConfigLayout = new QVBoxLayout();
    QWidget noiseSourceEditor = new NoiseSourceEditor(nose->getNoiseSource());

    noiseSrcConfigLayout->addWidget(noiseSourceEditor);
    noiseSourcesGroupBox->setLayout(noiseSrcConfigLayout);

    QVBoxLayout * groupBoxesLayout = new QVBoxLayout();
    groupBoxesLayout->addWidget(outletSettings);
    groupBoxesLayout->addWidget(netDeviceGroupBox);
    groupBoxesLayout->addWidget(noiseSourceGroupBox);

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
}

void NodeConfiguration::setModel(NodeModel *newModel){
    nodeModel = newModel;
    populateFromModel();
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
