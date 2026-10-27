#ifndef SATELLITE_H
#define SATELLITE_H

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

#include "QDoubleEdit.h"
#include "QSpeedEdit.h"
#include "QDistanceEdit.h"
#include "QTimeValueEdit.h"

#include "FsSpace.h"


class Satellite : public QWidget
{
    Q_OBJECT
public:
    explicit Satellite(QWidget *parent = 0);

signals:

public slots:
    void changedHeight();
    void changedSpeed();

    void setCaption(QString s);
private:
    bool refFlag;
    QDistanceEdit     *edRadius;
    QDistanceEdit     *edGroundHeight;

    QSpeedEdit     *edSpeed;
    QTimeValueEdit     *edRoundTime;
    QLabel          *lbCaption;

};

#endif // SATELLITE_H
