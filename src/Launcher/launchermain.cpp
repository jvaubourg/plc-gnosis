#include <QDebug>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDir>

int main(int argc, char* argv[]){

    if(argc == 0){
        qDebug() << "Pass name of ns3 program to launch.";
    }

    QProcess* scriptProcess = new QProcess();
    scriptProcess->setProcessChannelMode(QProcess::ForwardedChannels);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    //Make sure we can get at the ns3 libraries
    env.insert(LD_ENV_VAR, QDir::currentPath() + ':' + PLC_DEFAULT_NS3_PATH + ':' + env.value(LD_ENV_VAR));
    scriptProcess->setProcessEnvironment(env);

    qDebug() << "GUI:\n    Running external simulator process";

    scriptProcess->start("./simulator");
    scriptProcess->waitForFinished();

    qDebug() << "Hello from launcher!";
}
