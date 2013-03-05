#include "plctopologyloader.h"
#include <QFile>

#include "qjson/parser.h"

#include "ns3/plc-spectrum-helper.h"
#include "ns3/object-factory.h"

PLCTopologyLoader::PLCTopologyLoader(PLCTopologyModel topologyModel){

    PLC_SpectrumModelHelper smHelper;

    double numBands = (topologyModel.getSpectrumModel().getUpperBandLimit() - topologyModel.getSpectrumModel().getLowerBandLimit());
    numBands /= topologyModel.getSpectrumModel().getBandResolution();

    Ptr<const SpectrumModel> sm = smHelper.GetSpectrumModel(topologyModel.getSpectrumModel().getLowerBandLimit(),
                                                      topologyModel.getSpectrumModel().getUpperBandLimit(),
                                                      numBands);

    PLC_Time::SetTimeModel(topologyModel.getSpectrumModel().getMainsFrequency(),
                           topologyModel.getSpectrumModel().getSamplesPerCycle(),
                           Seconds(topologyModel.getSpectrumModel().getSimulationLength()));

    plcGraph = Create<PLC_Graph>();

    // Create channel
    channel = CreateObject<PLC_Channel> ();
    channel->SetGraph(plcGraph);

    QList<NodeModel*>* topologyNodes = topologyModel.getNodes();
    QList<EdgeModel*>* topologyEdges = topologyModel.getEdges();


    //Loop through the nodes, and create as necessary.
    for(int i = 0; i < topologyNodes->length(); i++){
        NodeModel* currentNode = topologyNodes->at(i);
        Ptr<PLC_Node> newNode = fromNodeModel(currentNode, sm);

        //Add the node to our tracking variables
        plcGraph->AddNode(newNode);
        nodesByName[currentNode->getName()] = newNode;

        //Loop through nodes net devices and create as necessary
        for(int j = 0; j < currentNode->netDevices()->length(); j++)
        {
            Ptr<PLC_NetDevice> newNetDevice = fromNetDeviceModel(currentNode->netDevices()->at(j), sm, newNode);
            QString netDeviceName = currentNode->netDevices()->at(j)->getName();

            this->netDevices.push_back(newNetDevice);
            this->netDeviceNames.push_back(netDeviceName);

            if(currentNode->netDevices()->at(j)->receiverEnabled()){
                this->receivers.push_back(newNetDevice);
                this->receiverNames.push_back(netDeviceName);
            }

            if(currentNode->netDevices()->at(j)->transmitterEnabled()){
                this->transmitters.push_back(newNetDevice);
                this->transmitterNames.push_back(netDeviceName);
            }
        }

        if(this->netDevices.size() > 0){
            this->netDeviceNodes.push_back(newNode);
        }

        //Loop Through Node noise sources and create as necessary.
        for(QList<NoiseSourceModel*>::iterator it = currentNode->noiseSources()->begin();
            it != currentNode->noiseSources()->end(); it++)
        {
            Ptr<PLC_NoiseSource> newNoiseSource = fromNoiseSourceModel((*it), sm, newNode);
        }

        if(currentNode->noiseSources()->length() != 0){
            this->noiseSources.push_back(newNode);
        }
    }

    ObjectFactory edgeFactory;
    //Loop through edges, create lines/two-ports and link to appropriate nodes
    for(int i = 0; i < topologyEdges->length(); i++){

        EdgeModel* currentEdge = topologyEdges->at(i);
        Ptr<PLC_Edge> newEdge;


        Ptr<PLC_Node> fromNode = nodesByName[currentEdge->getFromNode()];
        Ptr<PLC_Node> toNode = nodesByName[currentEdge->getToNode()];

        //Fake the node positions to avoid the user having to accurately depict the topology... (Hopefully this works!)
        toNode->SetPosition(currentEdge->getLength(), 0, 0);

        if(currentEdge->isCableModel()){
            edgeFactory.SetTypeId(currentEdge->getCableType().toStdString());
            Ptr<PLC_Cable> cable = edgeFactory.Create()->GetObject<PLC_Cable>();
            cable->SetSpectrumModel(sm);
            cable->Calculate();

            //fromCableFile(currentEdge->getCableType(), sm);
            newEdge = Create<PLC_Line>(cable, fromNode, toNode);
        }
        else{
            newEdge = Create<PLC_TwoPort>(sm, fromNode, toNode,
                                          fromValueString(currentEdge->getTwoPortParameters()[0], sm),
                                          fromValueString(currentEdge->getTwoPortParameters()[1], sm),
                                          fromValueString(currentEdge->getTwoPortParameters()[2], sm),
                                          fromValueString(currentEdge->getTwoPortParameters()[3], sm));
        }

        //Reset the 'to' nodes original position.
        edges.push_back(newEdge);
        toNode->SetPosition(0, 0, 0);
    }

    //And.... that should be everything...
}

Ptr<PLC_ValueBase> PLCTopologyLoader::infinity(Ptr<const SpectrumModel> spectrumModel){
    std::complex<double> infinity(std::numeric_limits<double>::infinity());
    return Create<PLC_ConstValue>(spectrumModel, infinity);
}

//TODO: Require as PLCValueString conversion to a SpectrumValue

//This function loads a csv file containing data (constant, time varying, frequency varying, or both) from a data file.
//The data file is just a CSV dump of two matrices, [A;B] where A is the REAL part of the data, and B is the IMAGINARY part
//of the data
Ptr<PLC_ValueBase> PLCTopologyLoader::valueFromFile(const QString& file, Ptr<const SpectrumModel> spectrumModel){
    QFile dataFile(file);

    dataFile.open(QIODevice::ReadOnly);
    QList<QByteArray> lines = dataFile.readAll().split('\n');

    if((lines.length() % 2) == 1){
        lines.pop_back();
    }

    int rows = lines.length();
    int cols = lines.at(0).split(',').length();

    QList<QList<double> > data;
    QList<double> currentRow;


    //Load data into doubles
    for(int i = 0; i < rows; i++){
        QList<QByteArray> values = lines.at(i).split(',');
        currentRow.clear();
        for(int j = 0; j < cols; j++){
            currentRow.push_back(values.at(j).toDouble());
        }
        data.push_back(currentRow);
    }

    //Load data into complex values
    std::vector< std::vector<std::complex<double> > >  complexValues;
    std::vector<std::complex<double> > currentComplexRow;

    for(int i = 0; i < (rows/2); i++){
        currentComplexRow.clear();
        for(int j = 0; j < cols; j++){
            currentComplexRow.push_back(std::complex<double>(data.at(i).at(j), data.at(i + (rows/2)).at(j)));
        }
        complexValues.push_back(currentComplexRow);
    }

    //We have half as many rows because we folded the second half into the imaginary part of complex values
    rows /= 2;

    if(rows == 1 || cols == 1){
        std::vector<complex<double> > valueVector;

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                valueVector.push_back(complexValues.at(i).at(j));
            }
        }

        //Return corresponding type
        if(rows == 1 && cols == 1){
            return Create<PLC_ConstValue>(spectrumModel, valueVector.at(0));
        }

        if(rows == 1){
            return Create<PLC_FreqSelectiveValue>(spectrumModel, valueVector);
        }

        if(cols == 1){
            return Create<PLC_TimeVariantConstValue>(spectrumModel, valueVector);
        }
    }


    return Create<PLC_TimeVariantFreqSelectiveValue>(spectrumModel, complexValues);

}

Ptr<PLC_ConstValue> PLCTopologyLoader::constFromValueString(PLCValueString value, Ptr<const SpectrumModel> spectrumModel){
    return Create<PLC_ConstValue>(spectrumModel, value.getComplex());
}

Ptr<PLC_ValueBase> PLCTopologyLoader::fromValueString(PLCValueString value, Ptr<const SpectrumModel> spectrumModel){
    if(value.isFile()){
        return valueFromFile(value.getValue(), spectrumModel);
    }
    else{
        return constFromValueString(value.getValue(), spectrumModel);
    }
}

Ptr<PLC_NoiseSource> PLCTopologyLoader::fromNoiseSourceModel(NoiseSourceModel *noiseSource, Ptr<const SpectrumModel> spectrumModel, Ptr<PLC_Node> sourceNode){

    Ptr<PLC_NoiseSource> newNoiseSource;

    Ptr<SpectrumValue> noisePSD = Create<SpectrumValue>(spectrumModel);
    (*noisePSD) = noiseSource->getNoisePSD().getReal();

    if(noiseSource->getNoiseType() == "Static"){
        newNoiseSource = Create<PLC_StaticNoiseSource>(sourceNode, noisePSD);
    }

    if(noiseSource->getNoiseType() == "Impulsive"){
        newNoiseSource = Create<PLC_ImpulsiveNoiseSource>(sourceNode, noisePSD);
    }

    //TODO: Adding on/off timing for the impulse noise source?

    return newNoiseSource;
}

Ptr<PLC_NetDevice> PLCTopologyLoader::fromNetDeviceModel(NetDeviceModel *netDevice, Ptr<const SpectrumModel> spectrumModel, Ptr<PLC_Node> sourceNode){

    ObjectFactory netDeviceFactory;
    netDeviceFactory.SetTypeId(PLC_SimpleNetDevice::GetTypeId());

    Ptr<PLC_NetDevice> newNetDevice = netDeviceFactory.Create<PLC_NetDevice>(); //TODO: More than just the simple net device type?

    ObjectFactory errorModelFactory;
    errorModelFactory.SetTypeId(PLC_ChannelCapacityErrorModel::GetTypeId());

    Ptr<SpectrumValue> txPsd = Create<SpectrumValue> (spectrumModel);
    (*txPsd) = 1e-7; // -40dBm

    /*Defaults for now -> Add to Net Device Configuration!!!*/
    newNetDevice->SetModulationAndCodingScheme(QPSK_1_2);
    newNetDevice->SetNoiseFloor(CreateBestCaseBgNoise(spectrumModel)->GetNoisePsd());
    newNetDevice->SetErrorModel(errorModelFactory.Create<PLC_ErrorModel>());
    newNetDevice->SetTxPowerSpectralDensity(txPsd);


    /*This stuff actually included from the diagram*/
    newNetDevice->SetPlcNode(sourceNode);

    if(!netDevice->getShuntImpedance().getValue().isEmpty()){
        newNetDevice->SetShuntImpedance(fromValueString(netDevice->getShuntImpedance(), spectrumModel));
    }

    newNetDevice->SetRxImpedance(fromValueString(netDevice->getRXImpedance(), spectrumModel));
    newNetDevice->SetTxImpedance(fromValueString(netDevice->getTXImpedance(), spectrumModel));

    newNetDevice->SetSpectrumModel(spectrumModel);


    //TODO: TX PSD, Device type, Address, Bridge, ErrorModel, Mod/Coding scheme.... maybe more??
    //Seems like shunt impedance gets thrown in as an outlet on the node.

    //TODO: See what 'creating a node' does. Doing it for now.... they are not saved anywhere however.
    Ptr<Node> newNetNode = CreateObject<Node>();
    newNetNode->AddDevice(newNetDevice);

    return newNetDevice;
}

Ptr<PLC_Node> PLCTopologyLoader::fromNodeModel(NodeModel *node, Ptr<const SpectrumModel> spectrumModel){
    Ptr<PLC_Node> newNode = Create<PLC_Node>();


    //Creating outlet might need to be done last or else net devices will overwrite it??
    if(node->getHasOutlet()){
        Ptr<PLC_Outlet> newOutlet = Create<PLC_Outlet>(newNode, fromValueString(node->getOutletImpedance(), spectrumModel));
    }

    //TODO: Double check that the position isn't used anywhere else other than when calculate is called on an edge...
    newNode->SetPosition(0, 0, 0);
    return newNode;
}

