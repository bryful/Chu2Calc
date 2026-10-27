#include "FreeFallDistance.h"

FreeFallDistance::FreeFallDistance(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("自由落下。落下距離から速度・時間を求める(近似値)"));
    //-----------
    edWeight    = new QWeightEdit;
    edDistance  = new QDistanceEdit;
    QLabel *lbAirDrag = new QLabel(FL8("空気抵抗係数 人:0.24 スポーツカー:0.3 球:0.5 平板:1.2 パラシュート1.42"));
    edAirDrag   = new QDoubleSpinBox;
    edAcl       = new QAccelerEdit;



    edTime      = new QTimeValueEdit;
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
    edDistance->setValue(2000);
    edAcl->setValue(GACL);
    edAcl->setMode(QWeightEdit::KG);

    edTime->setReadOnly(true);
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
    vl->addWidget(edDistance);
    vl->addWidget(lbAirDrag);
    vl->addWidget(edAirDrag);
    vl->addWidget(edAcl);
    vl->addSpacing(24);
    vl->addWidget(edTime);
    vl->addWidget(edSpeed);
    vl->addWidget(edMax);

    vl->addStretch();


    this->setLayout(vl);
    changedValue();
    connect(edWeight,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edDistance,SIGNAL(changedValue(double)),this,SLOT(changedValue()));
    connect(edAirDrag,SIGNAL(valueChanged(double)),this,SLOT(changedValue()));
    connect(edAcl,SIGNAL(changedValue(double)),this,SLOT(changedValue()));

}
//*********************************************
double FreeFallDistance::acosh(double b)
{
    return log(b + sqrt(b+1)*sqrt(b-1));
}

//*********************************************
void FreeFallDistance::changedValue()
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
    long double h = edDistance->value();
    long double v =0;
    long double t =0;
    long double A = 0;
    long double mx = 0;


    if (h>0){
        mx = sqrt(m*g/k);
        A = sqrt(m/(g*k));
        t = A*acosh(exp(h*k/m));
        v = mx * tanh(t/A);
    }
    edTime->setValue(t);
    edSpeed->setMPS(v);
    edMax->setMPS(mx);
    refFlag = false;

}
//*********************************************
void FreeFallDistance::changedValueNoDrag()
{
    if (refFlag) return;
    refFlag = true;
    long double g = edAcl->value();
    if (g<=0) {
        g = GACL;
        edAcl->setValue(g);
    }
    long double h = edDistance->value();
    long double v =0;
    long double t =0;


    t = sqrt(2* h/g);
    v = sqrt(2*g*h);
    edTime->setValue(t);
    edSpeed->setMPS(v);
    edMax->setMPS(0);
    refFlag = false;
}
//********************************************************
void FreeFallDistance::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
