#include "netdevicedialog.h"
#include <QHBoxLayout>

NetDeviceDialog::NetDeviceDialog(NetDeviceModel* device, QListWidgetItem* item, QWidget *parent) :
    QDialog(parent)
{
    netDevModel = device;
    listItem = item;

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

    QVBoxLayout * dialogLayout = new QVBoxLayout();
    dialogLayout->addLayout(nameLayout);

    dialogLayout->addWidget(shuntImpedanceInput);
    dialogLayout->addWidget(txImpedanceInput);
    dialogLayout->addWidget(rxImpedanceInput);


    this->setLayout(dialogLayout);

    QHBoxLayout * buttonLayout = new QHBoxLayout();
    QPushButton * acceptButton = new QPushButton("Accept", this);
    QPushButton * deleteButton = new QPushButton("Remove");
    //deleteButton->setFixedWidth(20);

    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(deleteButton);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));

    dialogLayout->addLayout(buttonLayout);

    populateFromModel();
}

void NetDeviceDialog::populateFromModel(){
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

void NetDeviceDialog::acceptClicked(){

    if(nameEdit->text().length() != 0 && txImpedanceInput->isValid() && rxImpedanceInput->isValid()
            && shuntImpedanceInput->isValid()){

        listItem->setText(nameEdit->text());

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

        accept();
    }
}

void NetDeviceDialog::deleteClicked(){
    done(-1);
}

void NetDeviceDialog::closeEvent(QCloseEvent *event){
    event->ignore();
}
