#ifndef DISTANCECONV_H
#define DISTANCECONV_H

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

#include "FsDistance.h"

class DistanceConv : public QWidget
{
    Q_OBJECT
public:
    explicit DistanceConv(QWidget *parent = 0);

    QString caption(){return lbCaption->text();}
signals:

public slots:
    void setValue(double d);
    void setCaption(QString s);
    void setReadOnly(bool b);

    void changedM(double d);
    void changedKM(double d);
    void changedTTKM(double d);
    void changedSND(double d);
    void changedSM(double d);
    void changedLS(double d);
    void changedLM(double d);
    void changedLH(double d);
    void changedLD(double d);
    void changedLY(double d);
    void changedAU(double d);
    void changedPC(double d);

    void changedFT(double d);
    void changedIC(double d);
    void changedYD(double d);
    void changedML(double d);
private:
    bool refFlag;
    QLabel          *lbCaption;
    QDoubleEdit     *edM;
    QDoubleEdit     *edKM;
    QDoubleEdit     *edTTKM;
    QDoubleEdit     *edSND;
    QDoubleEdit     *edSEAMILE;

    QDoubleEdit     *edFeet;
    QDoubleEdit     *edInch;
    QDoubleEdit     *edYard;
    QDoubleEdit     *edMile;

    QDoubleEdit     *edLightSec;
    QDoubleEdit     *edLightMin;
    QDoubleEdit     *edLightHour;
    QDoubleEdit     *edLightDay;
    QDoubleEdit     *edLightYear;
    QDoubleEdit     *edAU;
    QDoubleEdit     *edPC;
};

#endif // DISTANCECONV_H
