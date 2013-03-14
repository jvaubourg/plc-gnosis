#include "noisesourcedialog.h"
#include <QBoxLayout>

NoiseSourceDialog::NoiseSourceDialog(NoiseSourceModel* source, QListWidgetItem* item, QWidget *parent) :
    QDialog(parent)
{
    listItem = item;
    noiseSrcModel = source;

    QLabel * nameLabel = new QLabel("Name:", this);
    nameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    nameEdit = new QLineEdit(this);
    QHBoxLayout * nameLayout = new QHBoxLayout();

    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameEdit);

    noiseTypeSelector = new QComboBox(this);
    noiseTypeSelector->addItem("Static");
    noiseTypeSelector->addItem("Impulsive");
    noiseTypeSelector->setFixedWidth(100);


    noisePSDInput = new PLCDataInputWidget("Noise PSD", this);

    QVBoxLayout * dialogLayout = new QVBoxLayout();
    dialogLayout->addLayout(nameLayout);
    dialogLayout->addWidget(noisePSDInput);
    dialogLayout->addWidget(noiseTypeSelector, 0, Qt::AlignCenter);


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

void NoiseSourceDialog::populateFromModel(){
    this->nameEdit->setText(noiseSrcModel->getName());
    this->noiseTypeSelector->setCurrentIndex(noiseTypeSelector->findText(noiseSrcModel->getNoiseType()));
    this->noisePSDInput->setValue(noiseSrcModel->getNoisePSD().getValue());
}

void NoiseSourceDialog::acceptClicked(){

    if(nameEdit->text().length() != 0 && noisePSDInput->isValid()){
        listItem->setText(nameEdit->text());

        this->noiseSrcModel->setName(nameEdit->text());
        this->noiseSrcModel->setNoisePSD(noisePSDInput->getValue());

        accept();
    }
}

void NoiseSourceDialog::deleteClicked(){
    done(-1);
}

void NoiseSourceDialog::closeEvent(QCloseEvent * event){
    event->ignore();
}
