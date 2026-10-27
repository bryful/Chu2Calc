#include "Satellite.h"

Satellite::Satellite(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("人工衛星の速度を求める"));

    edRadius = new QDistanceEdit;
    edRadius->setValue(6371*1000);
    edRadius->setMode(QDistanceEdit::K);
    edRadius->setCaption(FL8("地球半径"));

    edGroundHeight = new QDistanceEdit;
    edGroundHeight->setValue(35786*1000);
    edGroundHeight->setMode(QDistanceEdit::K);
    edGroundHeight->setCaption(FL8("地上高"));

    edSpeed = new QSpeedEdit;
    edSpeed->setCaption(FL8("衛星速度"));

    edRoundTime = new QTimeValueEdit;
    edRoundTime->setReadOnly(true);
    edRoundTime->setCaption(FL8("周回時間"));



    QString d;
    d  = FL8("対流圏:\t0 - 9/17km(飛行機が飛べる)\n");
    d  = FL8("成層圏:\t9/17 - 50km(気球が上がれる。オゾン層)\n");
    d += FL8("中間圏:\t50 - 85km(流星)\n");
    d += FL8("カーマンライン:\t100km(ここから宇宙扱い)\n");
    d += FL8("国際宇宙ステーションISS\t15km\n");
    d += FL8("　熱圏:\t80 - 約800/1000km(スペースシャトル・人工衛星)\n");
    d += FL8("外気圏:\t10000km\n");
    d += FL8("低軌道(LEO)\t2000km以下\n");
    d += FL8("中軌道(MEO)\t2000km - 地球同期軌道(35786km)まで\n");
    d += FL8("高軌道(HEO)\t地球同期軌道(35786km)以上\n");
    d += FL8("月軌道\t384400km\n");

    QLabel *detail = new QLabel(d);
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edRadius);
    vl->addWidget(edGroundHeight);
    vl->addWidget(edSpeed);
    vl->addWidget(edRoundTime);
    vl->addWidget(detail);

    vl->addStretch();
    this->setLayout(vl);

    connect(edRadius,SIGNAL(changedValue(double)),this,SLOT(changedHeight()));
    connect(edGroundHeight,SIGNAL(changedValue(double)),this,SLOT(changedHeight()));
    connect(edSpeed,SIGNAL(changedValue(double)),this,SLOT(changedSpeed()));
    changedHeight();
}
//***********************************************************************
void Satellite::changedHeight()
{
    if (refFlag) return;
    refFlag = true;
    double r = edRadius->value();
    double h = edGroundHeight->value();

    double v = sqrt(GACL*pow(r,2)/(r + h));
    edSpeed->setMPS(v);

    double rd = M_PI * (r+h)*2;
    double t = rd / v;
    edRoundTime->setValue(t);

    refFlag = false;
}
//***********************************************************************
void Satellite::changedSpeed()
{
    if (refFlag) return;
    refFlag = true;
    double r = edRadius->value();
    double v = edSpeed->value();

    double h = GACL*pow(r,2)/pow(v,2) -r;
    edGroundHeight->setValue(h);

    double rd = M_PI * (r+h)*2;
    double t = rd / v;
    edRoundTime->setValue(t);

    refFlag = false;
}

//********************************************************
void Satellite::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
