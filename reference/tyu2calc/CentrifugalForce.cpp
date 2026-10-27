#include "CentrifugalForce.h"

CentrifugalForce::CentrifugalForce(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("回転半径と回転数から、遠心重力を求める"));

    edRadius    = new QDistanceEdit;
    edTime      = new QTimeValueEdit;
    edG         = new QAccelerEdit;
    edSpeed     = new QSpeedEdit;

    edRadius->setCaption(FL8("回転半径"));
    edTime->setCaption(FL8("１回転する時間"));
    edG->setCaption(FL8("遠心加速度"));
    edSpeed->setCaption(FL8("接線速度"));

    rbUnFixedRadius = new QRadioButton(FL8("半径を変える"));
    rbUnFixedTime = new QRadioButton(FL8("時間を変える"));
    rbUnFixedTime->setChecked(true);


    QHBoxLayout *hl = new QHBoxLayout;
    hl->addSpacing(25);
    hl->addWidget(rbUnFixedRadius);
    hl->addWidget(rbUnFixedTime);
    hl->addStretch();

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edRadius);
    vl->addWidget(edTime);
    vl->addSpacing(25);
    vl->addWidget(edG);
    vl->addWidget(edSpeed);
    vl->addLayout(hl);
    vl->addStretch();

    this->setLayout(vl);
    edRadius->setValue(3000);
    edTime->setValue(110);
    changedValue();
    connect(edRadius,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edTime,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edG,SIGNAL(changedValue(double)),this,SLOT(changedG(double)));
    connect(edSpeed,SIGNAL(changedValue(double)),this,SLOT(changedSpeed(double)));
}
//***********************************************************
void CentrifugalForce::changedValue()
{
    if (refFlag) return;
    refFlag = true;
    double a = 0;
    double v = 0;
    if (edTime->value() != 0){
        a = pow(2 *M_PI / edTime->value(),2) * edRadius->value();
        v = (2 *M_PI / edTime->value()) * edRadius->value();
    }

    edG->setValue(a);
    edSpeed->setMPS(v);
    refFlag = false;

}
//***********************************************************
void CentrifugalForce::changedG(double d)
{
    if (refFlag) return;
    refFlag = true;

    if(rbUnFixedTime->isChecked()){
        double t = 0;
        if ((d!=0)&&(edRadius->value()!=0)){
            t = (2 *M_PI) / sqrt(d / edRadius->value());
        }
        edTime->setValue(t);
    } else if (rbUnFixedRadius->isChecked()){
        double r = 0;
        if ((d!=0)&&(edTime->value()!=0)){
            r = d /pow(2*M_PI/edTime->value(),2);
        }
        edRadius->setValue(r);
    }
    refFlag = false;
}
//***********************************************************
void CentrifugalForce::changedSpeed(double d)
{
    if (refFlag) return;
    refFlag = true;

    if(rbUnFixedTime->isChecked()){
        double t = 0;
        if (d!=0){
            t = (2 *M_PI) * edRadius->value()/ d ;
        }
        edTime->setValue(t);
    } else if (rbUnFixedRadius->isChecked()){
        double r = 0;
        if (edTime->value()!=0){
            r = d / (2*M_PI/edTime->value());
        }
        edRadius->setValue(r);
    }
    refFlag = false;
}
//********************************************************
void CentrifugalForce::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
