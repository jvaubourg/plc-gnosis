#ifndef PLCTOPOLOGYLOADER_H
#define PLCTOPOLOGYLOADER_H

#include <ns3/plc.h>
#include <ns3/object.h>
#include <ns3/ptr.h>

#include <QStringList>
#include <QDebug>

#include <complex>

#include "plctopologymodel.h"

#include "plctopologyloader_export.h"

using namespace ns3;
using namespace std;

class PLCTopologyLoader
{
public:
    PLCTopologyLoader(PLCTopologyModel topologyModel);

    static Ptr<PLC_Node>        fromNodeModel(NodeModel* node, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_Edge>        fromEdgeModel(EdgeModel* edge, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_NetDevice>   fromNetDeviceModel(NetDeviceModel* netDevice, Ptr<const SpectrumModel> spectrumModel, Ptr<PLC_Node> sourceNode);
    static Ptr<PLC_NoiseSource> fromNoiseSourceModel(NoiseSourceModel* noiseSource, Ptr<const SpectrumModel> spectrumModel, Ptr<PLC_Node> sourceNode);

    static Ptr<PLC_ValueBase>   infinity(Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_ValueBase>   valueFromFile(const QString& file, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_ConstValue>  constFromValueString(PLCValueString value, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_ValueBase>   fromValueString(PLCValueString value, Ptr<const SpectrumModel> spectrumModel);

    Ptr<PLC_Channel> getChannel(){return channel;}
    PLC_NetdeviceList getNetDevices(){return netDevices;}

    PLC_NodeList getNetDeviceNodes() {return netDeviceNodes;}
    QStringList  getNetDeviceNames(){return netDeviceNames;}

    PLC_NetdeviceList getReceivers() {return receivers;}
    QStringList  getReceiverNames() {return receiverNames;}

    PLC_NetdeviceList getTransmitters() {return transmitters;}
    QStringList  getTransmitterNames(){return transmitterNames;}

    Ptr<SpectrumModel> getSpectrumModel(){ return spectrumModel;}

private:

    Ptr<SpectrumModel> spectrumModel;

    QMap<QString, Ptr<PLC_Node> > nodesByName;

    PLC_NodeList netDeviceNodes;
    PLC_NodeList noiseSources;

    PLC_NetdeviceList netDevices;
    QStringList netDeviceNames;

    PLC_NetdeviceList transmitters;
    QStringList transmitterNames;

    PLC_NetdeviceList receivers;
    QStringList receiverNames;

    QList< Ptr<PLC_Edge> > edges;

    Ptr<PLC_Graph> plcGraph;

    Ptr<PLC_Channel> channel;
};

#endif // PLCTOPOLOGYLOADER_H
