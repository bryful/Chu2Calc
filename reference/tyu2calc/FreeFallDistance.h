#ifndef FREEFALLDISTANCE_H
#define FREEFALLDISTANCE_H

#include <QWidget>

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


class FreeFallDistance : public QWidget
{
    Q_OBJECT
public:
    explicit FreeFallDistance(QWidget *parent = 0);

signals:

public slots:
    void changedValue();
    void changedValueNoDrag();

    void setCaption(QString s);
private:
    bool refFlag;
    QWeightEdit     *edWeight;
    QDistanceEdit   *edDistance;
    QDoubleSpinBox  *edAirDrag;
    QAccelerEdit    *edAcl;

    QTimeValueEdit  *edTime;
    QSpeedEdit      *edSpeed;
    QSpeedEdit      *edMax;

    double acosh(double b);
    QLabel          *lbCaption;

};

#endif // FREEFALLDISTANCE_H
