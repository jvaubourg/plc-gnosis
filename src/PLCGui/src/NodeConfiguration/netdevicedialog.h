/*

Copyright (C) 2013 Donald Harris (dharris.ubc@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>

*/

#ifndef NETDEVICEDIALOG_H
#define NETDEVICEDIALOG_H

#include <QDialog>
#include "netdevicemodel.h"
#include "plcdatainputwidget.h"
#include <QListWidgetItem>
#include <QCloseEvent>

class NetDeviceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NetDeviceDialog(NetDeviceModel* device, QListWidgetItem* item, QWidget *parent = 0);

signals:
    
public slots:
    void acceptClicked();
    void deleteClicked();

protected:
    virtual void closeEvent(QCloseEvent * event);

private:
    void populateFromModel();
    //TODO dropdown for different/more types??
    QLineEdit * nameEdit;

    PLCDataInputWidget * shuntImpedanceInput;
    PLCDataInputWidget * txImpedanceInput;
    PLCDataInputWidget * rxImpedanceInput;

    NetDeviceModel* netDevModel;
    QListWidgetItem* listItem;
};

#endif // NETDEVICEDIALOG_H
