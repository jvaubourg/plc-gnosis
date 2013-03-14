#include "nodeconfiguration.h"
#include <QHBoxLayout>

#include <QListWidget>
#include <QPushButton>

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

    netDeviceSettings = new QGroupBox("Net Device");
    netDeviceSettings->setCheckable(true);


    QVBoxLayout* netDevConfigLayout = new QVBoxLayout();
    netDeviceEditor = new NetDeviceEditor(node->getNetDevice(), this);

    netDevConfigLayout->addWidget(netDeviceEditor);
    netDeviceSettings->setLayout(netDevConfigLayout);

    noiseSourceSettings = new QGroupBox("Noise Source");
    noiseSourceSettings->setCheckable(true);

    QVBoxLayout* noiseSrcConfigLayout = new QVBoxLayout();
    noiseSourceEditor = new NoiseSourceEditor(node->getNoiseSource(), this);

    noiseSrcConfigLayout->addWidget(noiseSourceEditor);
    noiseSourceSettings->setLayout(noiseSrcConfigLayout);

    QVBoxLayout * groupBoxesLayout = new QVBoxLayout();
    groupBoxesLayout->addWidget(outletSettings);
    groupBoxesLayout->addWidget(netDeviceSettings);
    groupBoxesLayout->addWidget(noiseSourceSettings);

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    QPushButton * closeButton = new QPushButton("Close");
    buttonsLayout->addWidget(closeButton);
    groupBoxesLayout->addLayout(buttonsLayout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

    this->setLayout(groupBoxesLayout);

    populateFromModel();

}

void NodeConfiguration::populateFromModel(){

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

    if(noiseSourceSettings->isChecked()){

    }

    if(netDeviceSettings->isChecked()){

    }

}
