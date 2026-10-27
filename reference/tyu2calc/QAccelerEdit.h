#ifndef QACCELEREDIT_H
#define QACCELEREDIT_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <float.h>

#include <QSize>

#include <QDoubleSpinBox>
#include <QKeyEvent>

#include <QtCore/qmath.h>

#include <QValidator>
#include <QDoubleValidator>

#include "FsSpace.h"

#include "FsAcceleration.h"

class QAccelerEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QAccelerEdit(QWidget *parent = 0);
    enum{
        G=0,
        MS2,
        KS2,
        TKS2,
        MM2,
        KM2,
        TKM2,
        MH2,
        KH2,
        TKH2,
    };

    int mode(){return cmbUnit->currentIndex();}
    bool isReadOnly(){return edBox->isReadOnly();}
    QString caption(){return lbCaption->text();}
    double value() { return m_value.MPS2();}

signals:
    void changedValue(double);

public slots:
    void setMode(int md);
    void setValue(double d);
    void setReadOnly(bool b);
    void setCaption(QString s);

private slots:
    void changedBox(double d);

private:
    bool    refFlag;
    FsAcceleration  m_value;
    QLabel          *lbCaption;
    QComboBox       *cmbUnit;
    QDoubleSpinBox  *edBox;

    void dispValue();

};

#endif // QACCELEREDIT_H
