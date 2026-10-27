#include "Acceleration1.h"

Acceleration1::Acceleration1(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("速度と時間から加速度を求める"));


    edFirstSpeed    = new QSpeedEdit;
    edFinalSpeed    = new QSpeedEdit;
    edTime          = new QTimeValueEdit;
    edAcl           = new QAccelerEdit;

    edFirstSpeed->setCaption(FL8("初期速度"));
    edFinalSpeed->setCaption(FL8("最終速度"));
    edTime->setCaption(FL8("時間"));
    edAcl->setCaption(FL8("加速度"));

    rbUnfixedFirst = new QRadioButton(FL8("初期速度を変更"));
    rbUnfixedFinal = new QRadioButton(FL8("最終速度を変更"));
    rbUnfixedTime = new QRadioButton(FL8("時間を変更"));
    rbUnfixedTime->setChecked(true);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addSpacing(25);
    hl->addWidget(rbUnfixedFirst);
    hl->addWidget(rbUnfixedFinal);
    hl->addWidget(rbUnfixedTime);
    hl->addStretch();


    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edFirstSpeed);
    vl->addWidget(edFinalSpeed);
    vl->addWidget(edTime);
    vl->addSpacing(15);
    vl->addWidget(edAcl);
    vl->addLayout(hl);
    vl->addStretch();

    this->setLayout(vl);
    connect(edFirstSpeed,SIGNAL(changedValue(double)),this,SLOT(changedSpeed()));
    connect(edFinalSpeed,SIGNAL(changedValue(double)),this,SLOT(changedSpeed()));
    connect(edTime,SIGNAL(changedValue(double)),this,SLOT(changedSpeed()));
    connect(edAcl,SIGNAL(changedValue(double)),this,SLOT(changedAcl()));

}
//********************************************************
void Acceleration1::changedSpeed()
{
    if (refFlag) return;
    refFlag = true;
    if (edTime->value()!=0){
       edAcl->setValue((edFinalSpeed->value() - edFirstSpeed->value())/edTime->value());
    }else{
        edAcl->setValue(0);
    }
    refFlag = false;
}
//********************************************************
void Acceleration1::changedAcl()
{
    if (refFlag) return;
    refFlag = true;
    if (rbUnfixedTime->isChecked()){
        if (edAcl->value()==0){
            edTime->setValue(0);
        }else{
            edTime->setValue((edFinalSpeed->value() - edFirstSpeed->value())/edAcl->value() );
        }
    }else if (rbUnfixedFirst->isChecked()){
            edFirstSpeed->setMPS(edFinalSpeed->value() - edAcl->value()*edTime->value() );
    }else if (rbUnfixedFinal->isChecked()){
            edFinalSpeed->setMPS(edFirstSpeed->value() + edAcl->value()*edTime->value() );
    }
    refFlag = false;
}
//********************************************************
void Acceleration1::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
