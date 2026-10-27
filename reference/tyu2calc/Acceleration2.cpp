#include "Acceleration2.h"

Acceleration2::Acceleration2(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;

    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("速度と距離から加速度を求める"));


    edSpeed     = new QSpeedEdit;
    edDistance  = new QDistanceEdit;
    edAcl       = new QAccelerEdit;
    edTime      = new QTimeValueEdit;
    edTime->setReadOnly(true);

    edSpeed->setCaption(FL8("速度"));
    edDistance->setCaption(FL8("距離"));
    edAcl->setCaption(FL8("加速度"));
    edTime->setCaption(FL8("時間"));

    rbUnfixedSpped = new QRadioButton(FL8("速度を変更"));
    rbUnfixedDistance = new QRadioButton(FL8("距離を変更"));
    rbUnfixedSpped->setChecked(true);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addSpacing(45);
    hl->addWidget(rbUnfixedSpped);
    hl->addWidget(rbUnfixedDistance);
    hl->addStretch();

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edDistance);
    vl->addWidget(edSpeed);
    vl->addSpacing(12);
    vl->addWidget(edAcl);
    vl->addWidget(edTime);
    vl->addLayout(hl);
    vl->addStretch();

    this->setLayout(vl);

    connect(edSpeed,SIGNAL(changedValue(double)),this,SLOT(changedSpeed()));
    connect(edDistance,SIGNAL(changedValue(double)),this,SLOT(changedSpeed()));
    connect(edAcl,SIGNAL(changedValue(double)),this,SLOT(changedAcl()));

}
//********************************************************
void Acceleration2::changedSpeed()
{
    if (refFlag) return;
    refFlag = true;
    if (edDistance->value()==0){
        edAcl->setValue(0);
        edTime->setValue(0);
    }else{
        edAcl->setValue( qPow( edSpeed->value(),2)/(2 * edDistance->value()) );
       if(edAcl->value()!=0){
        edTime->setValue(qSqrt(2* edDistance->value()/edAcl->value()));
       }else{
           edTime->setValue(0);
       }
    }

    refFlag = false;
}
//********************************************************
void Acceleration2::changedAcl()
{
    if (refFlag) return;
    refFlag = true;
    if (rbUnfixedSpped->isChecked()){
        edSpeed->setMPS(qSqrt(2* edDistance->value()*edAcl->value()));
    }else{
        edDistance->setValue(qPow(edSpeed->value(),2)/(2*edAcl->value()) );
    }

    if(edAcl->value()!=0){
     edTime->setValue(qSqrt(2* edDistance->value()/edAcl->value()));
    }else{
        edTime->setValue(0);
    }

    refFlag = false;
}
//********************************************************
void Acceleration2::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
//********************************************************
