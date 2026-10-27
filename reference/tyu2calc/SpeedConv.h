#ifndef SPEEDCONV_H
#define SPEEDCONV_H

#include <QWidget>

#include <QtCore/qmath.h>

#include <QValidator>
#include <QDoubleValidator>

#include <QLabel>
#include <QLineEdit>

#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTextEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QDoubleSpinBox>
#include "QDoubleEdit.h"

#include "FsSpace.h"

#include "FsSpeed.h"

class SpeedConv : public QWidget
{
    Q_OBJECT
public:
    explicit SpeedConv(QWidget *parent = 0);

    QString caption(){return lbCaption->text();}
signals:
    void valueChanged(double);

public slots:
    void setValue(double d);
    void changedMPS(double d);
    void changedKPS(double d);
    void changedMPM(double d);
    void changedKPM(double d);
    void changedMPH(double d);
    void changedKPH(double d);
    void changedTTKPH(double d);

    void changedMach(double d);
    void changedLight(double d);

    void changedKNOT(double d);

    void setReadOnly(bool b);
    void setCaption(QString s);
private:
    bool refFlag;
    QLabel        *lbCaption;
    QDoubleEdit   *edMPSec;   //秒速
    QDoubleEdit   *edMPMin;   //分速
    QDoubleEdit   *edMPHour;  //時速
    QDoubleEdit   *edMach;    //音速
    QDoubleEdit   *edKnot;    //ノット
    QDoubleEdit   *edLight;   //光速

    QDoubleEdit   *edKPSec;   //秒速
    QDoubleEdit   *edKPMin;   //分速
    QDoubleEdit   *edKPHour;  //時速
    QDoubleEdit   *edTTKPHour;  //時速




};

#endif // SPEEDCONV_H
