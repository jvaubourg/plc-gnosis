#ifndef PLCTOPOLOGYLOADER_H
#define PLCTOPOLOGYLOADER_H

#include <ns3/plc.h>
#include <ns3/object.h>
#include <ns3/ptr.h>

#include <QtCore/qglobal.h>
#include <QStringList>
#include <QDebug>

#include <complex>

#include "plctopologymodel.h"

#ifndef PLCTOPOLOGYLOADER_EXPORT
    #if defined(PLCTOPOLOGYMODEL_MAKEDLL)
        /* We are building this library */
        #define PLCTOPOLOGYLOADER_EXPORT Q_DECL_EXPORT

    #else
        /* We are using this library */
        #define PLCTOPOLOGYLOADER_EXPORT Q_DECL_IMPORT

    # endif
#endif

using namespace ns3;
using namespace std;

class PLCTopologyLoader
{
public:
    PLCTopologyLoader(PLCTopologyModel topologyModel);

    static Ptr<PLC_Node>        fromNodeModel(NodeModel* node, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_Edge>        fromEdgeModel(EdgeModel* edge, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_NetDevice>   fromNetDeviceModel(NetDeviceModel* netDevice, Ptr<const SpectrumModel> spectrumModel, Ptr<PLC_Node> sourceNode, Mac48Address addr);
    static Ptr<PLC_NoiseSource> fromNoiseSourceModel(NoiseSourceModel* noiseSource, Ptr<const SpectrumModel> spectrumModel, Ptr<PLC_Node> sourceNode);

    static Ptr<PLC_ValueBase>   infinity(Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_ValueBase>   valueFromFile(const QString& file, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_ConstValue>  constFromValueString(PLCValueString value, Ptr<const SpectrumModel> spectrumModel);
    static Ptr<PLC_ValueBase>   fromValueString(PLCValueString value, Ptr<const SpectrumModel> spectrumModel);

    static void incrementMacAddress(Mac48Address *addr);

    Ptr<PLC_Channel> getChannel(){return channel;}
    PLC_NetdeviceList getNetDevices(){return netDevices;}

    PLC_NodeList getNetDeviceNodes() {return netDeviceNodes;}
    QStringList  getNetDeviceNames(){return netDeviceNames;}

    PLC_NetdeviceList getReceivers() {return receivers;}
    QStringList  getReceiverNames() {return receiverNames;}

    PLC_NetdeviceList getTransmitters() {return transmitters;}
    QStringList  getTransmitterNames(){return transmitterNames;}

    Ptr<SpectrumModel> getSpectrumModel(){ return spectrumModel;}

    Ptr<PLC_NetDevice> getTransmitterByName(QString name){ return transmitters.at(transmitterNameToIndexMap.value(name)); }
    Ptr<PLC_NetDevice> getReceiverByName(QString name){ return receivers.at(receiverNameToIndexMap.value(name)); }

private:

    Ptr<SpectrumModel> spectrumModel;

    QMap<QString, Ptr<PLC_Node> > nodesByName;

    PLC_NodeList netDeviceNodes;
    PLC_NodeList noiseSources;

    PLC_NetdeviceList netDevices;
    QStringList netDeviceNames;

    PLC_NetdeviceList transmitters;
    QStringList transmitterNames;
    QMap<QString, int> transmitterNameToIndexMap;

    PLC_NetdeviceList receivers;
    QStringList receiverNames;
    QMap<QString, int> receiverNameToIndexMap;

    QList< Ptr<PLC_Edge> > edges;

    Ptr<PLC_Graph> plcGraph;

    Ptr<PLC_Channel> channel;
};

#endif // PLCTOPOLOGYLOADER_H
