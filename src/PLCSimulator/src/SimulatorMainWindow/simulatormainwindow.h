#ifndef SIMULATORMAINWINDOW_H
#define SIMULATORMAINWINDOW_H

#include <QMainWindow>
#include "../BodeWidget/bodewidget.h"

class SimulatorMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimulatorMainWindow(QWidget *parent = 0);
    BodeWidget * plot;

signals:
    
public slots:
    void saveMagnitudePlotAsPDF();

private:

    
};

#endif // SIMULATORMAINWINDOW_H
