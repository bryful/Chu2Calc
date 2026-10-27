#include "AccelaTime.h"

AccelaTime::AccelaTime(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;

    lbCaption = new QLabel;
    setCaption(FL8("加速時間から速度・距離を計算"));

    edAcceler = new QAccelerEdit;
    edAcceler->setCaption(FL8("加速度"));

    edTime = new QTimeValueEdit;
    edTime->setCaption(FL8("継続時間"));

    edSpeed = new QSpeedEdit;
    edSpeed->setReadOnly(true);
    edSpeed->setCaption(FL8("速度"));

    edDsitance = new QDistanceEdit;
    edDsitance->setReadOnly(true);
    edDsitance->setCaption(FL8("移動距離"));

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edAcceler);
    vl->addWidget(edTime);
    vl->addSpacing(5);

    vl->addWidget(edSpeed);
    vl->addWidget(edDsitance);
    vl->addStretch();

    this->setLayout(vl);

    connect(edAcceler,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edTime,SIGNAL(changedValue(double)),this,SLOT(changedValue()));

}
//--------------------------------------------------------
void AccelaTime::changedValue()
{
    double ac = edAcceler->value();
    double t = edTime->value();

    double spd = ac * t;
    double dis = ac * qPow(t,2)/2;

    edSpeed->setMPS(spd);
    edDsitance->setValue(dis);
}
//--------------------------------------------------------
void AccelaTime::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
//--------------------------------------------------------
