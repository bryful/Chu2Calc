#include "FreeFallTime.h"

FreeFallTime::FreeFallTime(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("自由落下。落下時間から速度・距離を求める(近似値)"));

    //-----------
    edWeight    = new QWeightEdit;
    edTime      = new QTimeValueEdit;
    QLabel *lbAirDrag = new QLabel(FL8("空気抵抗係数 人:0.24 スポーツカー:0.3 球:0.5 平板:1.2 パラシュート1.42"));
    edAirDrag   = new QDoubleSpinBox;
    edAcl       = new QAccelerEdit;
    edDistance  = new QDistanceEdit;
    edSpeed     = new QSpeedEdit;
    edMax     = new QSpeedEdit;

    edMax->setCaption(FL8("終端速度"));

    lbAirDrag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QFont f = edAirDrag->font();
    f.setBold(true);
    f.setPointSize(12);
    edAirDrag->setFont(f);

    edWeight->setValue(72*1000);
    edWeight->setMode(QWeightEdit::KG);
    edTime->setValue(40);

    edAcl->setValue(GACL);
    edAcl->setMode(QWeightEdit::KG);

    edDistance->setReadOnly(true);
    edSpeed->setReadOnly(true);
    edMax->setReadOnly(true);

    edAirDrag->setMinimum(-DBL_MAX);
    edAirDrag->setMaximum(DBL_MAX);
    edAirDrag->setMinimumWidth(30);
    edAirDrag->setMaximumWidth(1280);
    edAirDrag->setDecimals(12);
    edAirDrag->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    edAirDrag->setValue(0.24);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edWeight);
    vl->addWidget(edTime);
    vl->addWidget(lbAirDrag);
    vl->addWidget(edAirDrag);
    vl->addWidget(edAcl);
    vl->addSpacing(24);
    vl->addWidget(edDistance);
    vl->addWidget(edSpeed);
    vl->addWidget(edMax);

    vl->addStretch();


    this->setLayout(vl);
    changedValue();
    connect(edWeight,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edTime,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edAirDrag,SIGNAL(valueChanged(double)),this,SLOT(changedValue()));
    connect(edAcl,SIGNAL(changedValue(double)),this,SLOT(changedValue()));


}
//*********************************************
void FreeFallTime::changedValue()
{
    if (refFlag) return;
    long double k = edAirDrag->value()*1000;
    if (k<=0){
        changedValueNoDrag();
        return;
    }
    refFlag = true;

    long double g = edAcl->value();
    if (g<=0) {
        g = GACL;
        edAcl->setValue(g);
    }
    long double m = edWeight->value();
    long double t = edTime->value();
    long double v =0;
    long double h =0;
    long double A = 0;

    long double mx = 0;

    if (t>0){
        mx = sqrt(m*g/k);
        A = sqrt(m/(g*k));
        h = (m/k)*log(cosh(t/A));
        v = mx * tanh(t/A);
    }
    edDistance->setValue(h);
    edSpeed->setMPS(v);
    edMax->setMPS(mx);
    refFlag = false;

}
//*********************************************
void FreeFallTime::changedValueNoDrag()
{
    if (refFlag) return;
    refFlag = true;

    long double g = edAcl->value();
    if (g<=0) {
        g = GACL;
        edAcl->setValue(g);
    }
    //long double m = edWeight->value();
    long double t = edTime->value();
    long double v =0;
    long double h =0;


    if (t>0){
        h = g*pow(t,2)/2;
        v = g*t;
    }
    edMax->setMPS(0);
    edDistance->setValue(h);
    edSpeed->setMPS(v);
    refFlag = false;

}
//********************************************************
void FreeFallTime::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
