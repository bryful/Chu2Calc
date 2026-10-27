#ifndef FREEFALL_H
#define FREEFALL_H

#include <QDebug>

#include <math.h>

#include <QWidget>
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
#include <QGroupBox>

#include <QDoubleSpinBox>

#include "QWeightEdit.h"
#include "QTimeValueEdit.h"
#include "QSpeedEdit.h"
#include "QAccelerEdit.h"
#include "QDistanceEdit.h"

#include "FsSpace.h"


class FreeFallTime : public QWidget
{
    Q_OBJECT
public:
    explicit FreeFallTime(QWidget *parent = 0);

signals:

public slots:
    void changedValue();
    void changedValueNoDrag();

    void setCaption(QString s);

private:
    bool refFlag;
    QWeightEdit     *edWeight;
    QTimeValueEdit  *edTime;
    QDoubleSpinBox  *edAirDrag;
    QAccelerEdit    *edAcl;
    QDistanceEdit   *edDistance;
    QSpeedEdit      *edSpeed;
    QSpeedEdit      *edMax;

    QLabel          *lbCaption;
};

#endif // FREEFALL_H
