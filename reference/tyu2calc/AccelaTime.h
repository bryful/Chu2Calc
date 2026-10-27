#ifndef ACCELATIME_H
#define ACCELATIME_H

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


class AccelaTime : public QWidget
{
    Q_OBJECT
public:
    explicit AccelaTime(QWidget *parent = 0);

signals:
    void changedG(bool mode);


public slots:
    void changedValue();
    void setCaption(QString s);
private:
    bool refFlag;
    QAccelerEdit    *edAcceler;
    QTimeValueEdit       *edTime;
    QSpeedEdit      *edSpeed;
    QDistanceEdit   *edDsitance;
    QLabel          *lbCaption;


};

#endif // ACCELATIME_H
