#ifndef QDISTANCEEDIT_H
#define QDISTANCEEDIT_H

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

#include "FsDistance.h"

class QDistanceEdit : public QWidget
{
    Q_OBJECT
public:
    explicit QDistanceEdit(QWidget *parent = 0);
    enum{
        M=0,
        K,
        TK,
        SS,
        SEAMILE,
        FEET,
        INCH,
        YARD,
        MILE,
        LS,
        LM,
        LH,
        LD,
        LY,
        AU,
        PC
    };
    int mode(){return cmbUnit->currentIndex();}
    bool isReadOnly(){return edBox->isReadOnly();}
    QString caption(){return lbCaption->text();}
    double value() { return m_value.M();}

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
    bool            refFlag;
    FsDistance      m_value;
    QLabel          *lbCaption;
    QComboBox       *cmbUnit;
    QDoubleSpinBox  *edBox;

    void dispValue();

};

#endif // QDISTANCEEDIT_H
