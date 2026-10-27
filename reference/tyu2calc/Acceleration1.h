#ifndef ACCELERATION1_H
#define ACCELERATION1_H

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
#include "QTimeValueEdit.h"
#include "QAccelerEdit.h"

#include "FsSpace.h"

class Acceleration1 : public QWidget
{
    Q_OBJECT
public:
    explicit Acceleration1(QWidget *parent = 0);

signals:

public slots:
    void changedSpeed();
    void changedAcl();

    void setCaption(QString s);

private:
    bool refFlag;
    QSpeedEdit      *edFirstSpeed;
    QSpeedEdit      *edFinalSpeed;
    QTimeValueEdit       *edTime;
    QAccelerEdit    *edAcl;
    QRadioButton    *rbUnfixedFirst;
    QRadioButton    *rbUnfixedFinal;
    QRadioButton    *rbUnfixedTime;

    QLabel          *lbCaption;

};

#endif // ACCELERATION1_H
