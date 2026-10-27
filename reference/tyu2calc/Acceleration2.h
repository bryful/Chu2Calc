#ifndef ACCELERATION2_H
#define ACCELERATION2_H

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTextEdit>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

#include "QSpeedEdit.h"
#include "QDistanceEdit.h"
#include "QTimeValueEdit.h"
#include "QAccelerEdit.h"

#include "FsSpace.h"


class Acceleration2 : public QWidget
{
    Q_OBJECT
public:
    explicit Acceleration2(QWidget *parent = 0);

signals:

public slots:
    void changedSpeed();
    void changedAcl();

    void setCaption(QString s);

private:
    bool refFlag;
    QSpeedEdit      *edSpeed;
    QDistanceEdit   *edDistance;
    QAccelerEdit    *edAcl;
    QTimeValueEdit       *edTime;
    QRadioButton    *rbUnfixedSpped;
    QRadioButton    *rbUnfixedDistance;
    QLabel          *lbCaption;

};

#endif // ACCELERATION2_H
