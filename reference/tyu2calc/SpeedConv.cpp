#include "SpeedConv.h"

//*******************************************************
SpeedConv::SpeedConv(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("速度単位変換"));

    edMPSec = new QDoubleEdit;
    edMPMin = new QDoubleEdit;
    edMPHour = new QDoubleEdit;
    edMach = new QDoubleEdit;
    edKnot = new QDoubleEdit;
    edLight = new QDoubleEdit;

    edKPSec = new QDoubleEdit;
    edKPMin = new QDoubleEdit;
    edKPHour = new QDoubleEdit;
    edTTKPHour = new QDoubleEdit;

    edMPSec->setSuffix(FL8(" m/秒"));
    edMPMin->setSuffix(FL8(" m/分"));
    edMPHour->setSuffix(FL8(" m/時"));
    edMach->setSuffix(FL8(" マッハ"));
    edKnot->setSuffix(FL8(" ノット"));
    edLight->setSuffix(FL8(" light/秒"));

    edKPSec->setSuffix(FL8(" km/秒"));
    edKPMin->setSuffix(FL8(" km/分"));
    edKPHour->setSuffix(FL8(" km/時"));
    edTTKPHour->setSuffix(" " + FL8("万km/時"));




    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(lbCaption);
    vl->addWidget(edMPSec);
    vl->addWidget(edKPSec);
    vl->addWidget(edMPMin);
    vl->addWidget(edKPMin);
    vl->addWidget(edMPHour);
    vl->addWidget(edKPHour);
    vl->addWidget(edTTKPHour);
    vl->addWidget(edMach);
    vl->addWidget(edKnot);
    vl->addWidget(edLight);

    vl->addStretch();

    this->setLayout(vl);

    connect(edMPSec,SIGNAL(valueChanged(double)),this,SLOT(changedMPS(double)));
    connect(edKPSec,SIGNAL(valueChanged(double)),this,SLOT(changedKPS(double)));
    connect(edMPMin,SIGNAL(valueChanged(double)),this,SLOT(changedMPM(double)));
    connect(edKPMin,SIGNAL(valueChanged(double)),this,SLOT(changedKPM(double)));
    connect(edMPHour,SIGNAL(valueChanged(double)),this,SLOT(changedMPH(double)));
    connect(edKPHour,SIGNAL(valueChanged(double)),this,SLOT(changedKPH(double)));
    connect(edTTKPHour,SIGNAL(valueChanged(double)),this,SLOT(changedTTKPH(double)));

    connect(edMach,SIGNAL(valueChanged(double)),this,SLOT(changedMach(double)));
    connect(edLight,SIGNAL(valueChanged(double)),this,SLOT(changedLight(double)));

    connect(edKnot,SIGNAL(valueChanged(double)),this,SLOT(changedKNOT(double)));
}
//*******************************************************
void SpeedConv::setValue(double d)
{
    edMPSec->setValue(d);
}
//*******************************************************
void SpeedConv::changedMPS(double d)
{
    if (refFlag) return;
    refFlag = true;

    FsSpeed fs;
    fs.setMPS(d);
    //edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::changedKPS(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setKMPS(d);
    edMPSec->setValue(fs.MPS());
    //edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;

}
//*******************************************************
void SpeedConv::changedMPM(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setMPM(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    //edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;

}
//*******************************************************
void SpeedConv::changedKPM(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setKMPM(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    //edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());

    refFlag = false;

}
//*******************************************************
void SpeedConv::changedMPH(double d){
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setMPH(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    //edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::changedKPH(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setKMPH(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    //edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::changedTTKPH(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setTKMPM(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    //edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::changedMach(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setMACH(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    //edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::changedKNOT(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setKNOT(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    //edKnot->setValue(fs.KNOT());
    edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::changedLight(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsSpeed fs;
    fs.setLS(d);
    edMPSec->setValue(fs.MPS());
    edKPSec->setValue(fs.KMPS());
    edMPMin->setValue(fs.MPM());
    edKPMin->setValue(fs.KMPM());
    edMPHour->setValue(fs.MPH());
    edKPHour->setValue(fs.KMPH());
    edTTKPHour->setValue(fs.TKMPH());
    edMach->setValue(fs.MACH());
    edKnot->setValue(fs.KNOT());
    //edLight->setValue(fs.LS());
    valueChanged(fs.MPS());
    refFlag = false;
}
//*******************************************************
void SpeedConv::setReadOnly(bool b)
{
    edMPSec->setReadOnly(b);   //秒速
    edMPMin->setReadOnly(b);   //分速
    edKPSec->setReadOnly(b);   //秒速
    edKPMin->setReadOnly(b);   //分速
    edKPHour->setReadOnly(b);  //時速
    edMPHour->setReadOnly(b);  //時速
    edTTKPHour->setReadOnly(b);  //時速
    edMach->setReadOnly(b);    //音速
    edLight->setReadOnly(b);   //光速
}

//*******************************************************
void SpeedConv::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
//*******************************************************


