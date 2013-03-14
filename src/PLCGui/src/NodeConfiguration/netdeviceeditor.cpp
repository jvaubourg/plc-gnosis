#include "netdeviceeditor.h"
#include <QHBoxLayout>

NetDeviceEditor::NetDeviceEditor(NetDeviceModel* device, QWidget *parent) :
    QWidget(parent)
{
    netDevModel = device;

    if(netDevModel == 0){
        netDevModel = new NetDeviceModel();
    }

    QLabel * nameLabel = new QLabel("Name:", this);
    nameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    nameEdit = new QLineEdit(this);
    QHBoxLayout * nameLayout = new QHBoxLayout();

    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameEdit);


    shuntImpedanceInput = new PLCDataInputWidget("Shunt Impedance", true, this);
    txImpedanceInput = new PLCDataInputWidget("TX Impedance", false, this);
    rxImpedanceInput = new PLCDataInputWidget("RX Impedance", false, this);

    txImpedanceInput->setCheckable(true);
    rxImpedanceInput->setCheckable(true);

    QVBoxLayout * editorLayout = new QVBoxLayout();
    editorLayout->addLayout(nameLayout);

    editorLayout->addWidget(shuntImpedanceInput);
    editorLayout->addWidget(txImpedanceInput);
    editorLayout->addWidget(rxImpedanceInput);


    this->setLayout(editorLayout);

    populateFromModel();
}

void NetDeviceEditor::populateFromModel(){
    this->nameEdit->setText(netDevModel->getName());
    this->shuntImpedanceInput->setValue(netDevModel->getShuntImpedance().getValue());

    if(shuntImpedanceInput->getValue().isEmpty()){
        shuntImpedanceInput->setChecked(false);
    }

    this->txImpedanceInput->setValue(netDevModel->getTXImpedance().getValue());
    this->rxImpedanceInput->setValue(netDevModel->getRXImpedance().getValue());

    this->rxImpedanceInput->setChecked(netDevModel->receiverEnabled());
    this->txImpedanceInput->setChecked(netDevModel->transmitterEnabled());
}

bool NetDeviceEditor::isValid(){
    return (nameEdit->text().length() != 0 && txImpedanceInput->isValid() && rxImpedanceInput->isValid()
            && shuntImpedanceInput->isValid());
}

void NetDeviceEditor::saveChanges(){

    if(isValid()){

        this->netDevModel->setName(nameEdit->text());
        this->netDevModel->setTXImpedance(txImpedanceInput->getValue());
        this->netDevModel->setRXImpedance(rxImpedanceInput->getValue());

        if(shuntImpedanceInput->isChecked()){
            this->netDevModel->setShuntImpedance(shuntImpedanceInput->getValue());
        }
        else{
            this->netDevModel->setShuntImpedance("");
        }

        this->netDevModel->setReceiverEnabled(rxImpedanceInput->isChecked());
        this->netDevModel->setTransmitterEnabled(txImpedanceInput->isChecked());
    }
}
