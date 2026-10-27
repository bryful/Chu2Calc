#ifndef CENTRIFUGALFORCE_H
#define CENTRIFUGALFORCE_H

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

#include "FsSpace.h"


#include "QDoubleEdit.h"
#include "SpeedConv.h"
#include "DistanceConv.h"

#include "QSpeedEdit.h"
#include "QDistanceEdit.h"
#include "QAccelerEdit.h"
#include "QTimeValueEdit.h"


class CentrifugalForce : public QWidget
{
    Q_OBJECT
public:
    explicit CentrifugalForce(QWidget *parent = 0);

signals:

public slots:
    void changedValue();
    void changedG(double d);
    void changedSpeed(double d);

    void setCaption(QString s);

private:
    bool refFlag;
    QDistanceEdit   *edRadius;
    QTimeValueEdit  *edTime;

    QAccelerEdit    *edG;
    QSpeedEdit      *edSpeed;
    QRadioButton    *rbUnFixedRadius;
    QRadioButton    *rbUnFixedTime;

    QLabel          *lbCaption;
};

#endif // CENTRIFUGALFORCE_H
