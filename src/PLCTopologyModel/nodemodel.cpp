#include "nodemodel.h"

NodeModel::NodeModel()
{
    hasOutlet = false;
}

NodeModel::NodeModel(const QVariantMap &map){
    fromVariantMap(map);
}

NodeModel::~NodeModel(){
    for(int i = 0; i < associatedNetDevices.length(); i++){
        delete(associatedNetDevices.at(i));
    }

    for(int i = 0; i < associatedNoiseSources.length(); i++){
        delete(associatedNoiseSources.at(i));
    }
}

QVariantMap NodeModel::toVariantMap(){
    QVariantMap map;

    QVariantList pos;
    QVariantList netDevices;
    QVariantList noiseSources;

    for(int i = 0; i < associatedNetDevices.length(); i++){
        netDevices << associatedNetDevices.at(i)->toVariantMap();
    }

    for(int i = 0; i < associatedNoiseSources.length(); i++){
        noiseSources << associatedNoiseSources.at(i)->toVariantMap();
    }

    pos << this->position.x() << this->position.y();

    map["Name"] = this->name;
    map["HasOutlet"] = this->hasOutlet;
    map["Position"] = pos;
    map["NetDevices"] = netDevices;
    map["NoiseSources"] = noiseSources;
    map["OutletImpedance"] = outletImpedance.getValue();

    return map;
}

void NodeModel::fromVariantMap(const QVariantMap& map){

    QVariantList pos = map["Position"].toList();
    QVariantList netDevices = map["NetDevices"].toList();
    QVariantList noiseSources = map["NoiseSources"].toList();

    name = map["Name"].toString();
    position = QPointF(pos.at(0).toDouble(), pos.at(1).toDouble());

    for(int i = 0; i < netDevices.length(); i++){
        this->associatedNetDevices.append(new NetDeviceModel(netDevices.at(i).toMap()));
    }

    for(int i = 0; i < noiseSources.length(); i++){
        this->associatedNoiseSources.append(new NoiseSourceModel(noiseSources.at(i).toMap()));
    }

    this->hasOutlet = map["HasOutlet"].toBool();
    this->outletImpedance.setValue(map["OutletImpedance"].toString());
}


