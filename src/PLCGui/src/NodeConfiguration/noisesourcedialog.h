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

#ifndef NOISESOURCEDIALOG_H
#define NOISESOURCEDIALOG_H

#include <QDialog>
#include "noisesourcemodel.h"
#include "plcdatainputwidget.h"
#include <QListWidgetItem>
#include <QComboBox>
#include <QCloseEvent>

class NoiseSourceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NoiseSourceDialog(NoiseSourceModel* source, QListWidgetItem* item, QWidget *parent);
    void populateFromModel();

signals:

protected:
    virtual void closeEvent(QCloseEvent *event);
    
public slots:
    void acceptClicked();
    void deleteClicked();

private:
    QComboBox* noiseTypeSelector;
    QLineEdit* nameEdit;
    PLCDataInputWidget * noisePSDInput;

    QListWidgetItem* listItem;

    NoiseSourceModel * noiseSrcModel;

    //TODO Investigate using a QVariantList for other parameters
};

#endif // NOISESOURCEDIALOG_H
