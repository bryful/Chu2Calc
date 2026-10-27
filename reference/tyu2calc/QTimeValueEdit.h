#ifndef QTIMEEDIT_H
#define QTIMEEDIT_H

#include <QDebug>

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

#include "FsTime.h"

class QTimeValueEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QTimeValueEdit(QWidget *parent = 0);
    enum{
        SEC=0,
        MIN,
        HOUR,
        DAY,
        YEAR,
        YDHMS
    };
    int mode(){return cmbUnit->currentIndex();}
    bool isReadOnly(){return edBox->isReadOnly();}
    QString caption(){return lbCaption->text();}
    double value() { return m_value.SEC();}

signals:
    void changedValue(double);

public slots:
    void setMode(int md);
    void setValue(double d);
    void setReadOnly(bool b);
    void setCaption(QString s);

private slots:
    void changedBox(double d);
    void changedBoxYEAR(int v);
    void changedBoxDAY(int v);
    void changedBoxHOUR(int v);
    void changedBoxMIN(int v);
    void changedBoxSEC(int v);
    void changedBoxMSEC(double v);

private:
    bool            refFlag;
    FsTime          m_value;
    QLabel          *lbCaption;
    QComboBox       *cmbUnit;
    QDoubleSpinBox  *edBox;

    QSpinBox        *edYear;
    QSpinBox        *edDay;
    QSpinBox        *edHour;
    QSpinBox        *edMin;
    QSpinBox        *edSec;
    QDoubleSpinBox  *edMSec;
    QLabel *lbY;
    QLabel *lbD;
    QLabel *lbH;
    QLabel *lbM;
    QLabel *lbS;
    QLabel *lbMS;

    QHBoxLayout     *hlYDHMS;

    void dispValue();
    void changeVisibled(bool b);
    QSpinBox* mkBox(int v = 60);


};

#endif // QTIMEEDIT_H
