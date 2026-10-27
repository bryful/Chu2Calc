#ifndef QWEIGHTEDIT_H
#define QWEIGHTEDIT_H

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

class QWeightEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QWeightEdit(QWidget *parent = 0);
    enum{
        G=0,
        KG,
        T,
        TTT
    };
    int mode(){return cmbUnit->currentIndex();}
    bool isReadOnly(){return edBox->isReadOnly();}
    QString caption(){return lbCaption->text();}
    double value() { return m_value;}

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
    double          m_value;
    QLabel          *lbCaption;
    QComboBox       *cmbUnit;
    QDoubleSpinBox  *edBox;

    void dispValue();

};

#endif // QWEIGHTEDIT_H
