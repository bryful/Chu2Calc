#ifndef QSPEEDEDIT_H
#define QSPEEDEDIT_H

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

#include "FsSpeed.h"

class QSpeedEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QSpeedEdit(QWidget *parent = 0);

    enum{
        MS=0,
        KS,
        MM,
        KM,
        MH,
        KH,
        TTKH,
        MACH,
        KNOT,
        LS,
        LM,
        LH,
        LD,
        LY
    };

    int mode(){return cmbUnit->currentIndex();}
    bool isReadOnly(){return edBox->isReadOnly();}
    QString caption(){return lbCaption->text();}
    double value() { return m_value.MPS();}

signals:
    void changedValue(double);

public slots:
    void setMode(int md);
    void setMPS(double d){ m_value.setMPS(d);dispValue();}
    void setReadOnly(bool b);
    void setCaption(QString s);

private slots:
    void changedBox(double d);

private:
    bool            refFlag;
    FsSpeed          m_value;
    QLabel          *lbCaption;
    QComboBox       *cmbUnit;
    QDoubleSpinBox  *edBox;

    void dispValue();
};

#endif // QSPEEDEDIT_H
