#ifndef PLCSPECTRUMMODEL_H
#define PLCSPECTRUMMODEL_H
#include <QVariantMap>

//All units are base (Hz, and seconds)
class PLCSpectrumModel
{
public:
    PLCSpectrumModel();
    PLCSpectrumModel(const QVariantMap& map);

    QVariantMap toVariantMap();
    void fromVariantMap(const QVariantMap& map);

    double getUpperBandLimit(){ return upperBandLimit; }
    double getLowerBandLimit(){ return lowerBandLimit; }
    double getBandResolution(){ return bandResolution; }

    int    getMainsFrequency(){ return mainsFrequency; }
    int    getSimulationLength(){ return simulationLength; }
    int    getSamplesPerCycle(){ return samplesPerCycle; }

    double getNoiseFloor(){ return noiseFloor; }

    void setUpperBandLimit(double limit) {upperBandLimit = limit;}
    void setLowerBandLimit(double limit) {lowerBandLimit = limit;}
    void setBandResolution(double resolution) {bandResolution = resolution; }

    void setMainsFrequency(int frequency) {mainsFrequency = frequency;}
    void setSimulationLength(int length)  {simulationLength = length;}
    void setSamplesPerCycle(int resolution) {samplesPerCycle = resolution;}

    void setNoiseFloor(double floor) { noiseFloor = floor;}

private:
    double upperBandLimit;
    double lowerBandLimit;
    double bandResolution;

    int mainsFrequency;
    int samplesPerCycle;
    int simulationLength;

    double noiseFloor;
};

#endif // PLCSPECTRUMMODEL_H
