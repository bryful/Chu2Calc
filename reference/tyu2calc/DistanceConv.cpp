#include "DistanceConv.h"

DistanceConv::DistanceConv(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;

    QVBoxLayout *vl = new QVBoxLayout;
    //--------------
    lbCaption = new QLabel;
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setCaption(FL8("距離単位変換"));
    //--------------
    edM         = new QDoubleEdit;
    edKM        = new QDoubleEdit;
    edTTKM      = new QDoubleEdit;
    edSND       = new QDoubleEdit;
    edSEAMILE   = new QDoubleEdit;

    edFeet      = new QDoubleEdit;
    edInch      = new QDoubleEdit;
    edYard      = new QDoubleEdit;
    edMile      = new QDoubleEdit;

    edLightSec  = new QDoubleEdit;
    edLightMin  = new QDoubleEdit;
    edLightHour = new QDoubleEdit;
    edLightDay  = new QDoubleEdit;
    edLightYear = new QDoubleEdit;
    edAU        = new QDoubleEdit;
    edPC        = new QDoubleEdit;

    edM->setSuffix(" " +D_M);
    edKM->setSuffix(" " +D_KM);
    edTTKM->setSuffix(" " + D_TKM +"a");
    edSND->setSuffix(" " +D_SS);
    edSEAMILE->setSuffix(" " +D_SM);

    edFeet->setSuffix(" " +D_FT);
    edInch->setSuffix(" " +D_IC);
    edYard->setSuffix(" " +D_YD);
    edMile->setSuffix(" " +D_ML);

    edLightSec->setSuffix(" " +D_LS);
    edLightMin->setSuffix(" " +D_LM);
    edLightHour->setSuffix(" " +D_LH);
    edLightDay->setSuffix(" " +D_LD);
    edLightYear->setSuffix(" " +D_LY);
    edAU->setSuffix(" " +D_AU);
    edPC->setSuffix(" " +D_PC);

    vl->addWidget(lbCaption);
    vl->addWidget(edM);
    vl->addWidget(edKM);
    vl->addWidget(edTTKM);
    vl->addWidget(edSND);
    vl->addWidget(edSEAMILE);

    vl->addWidget(edFeet);
    vl->addWidget(edInch);
    vl->addWidget(edYard);
    vl->addWidget(edMile);

    vl->addWidget(edLightSec);
    vl->addWidget(edLightMin);
    vl->addWidget(edLightHour);
    vl->addWidget(edLightDay);
    vl->addWidget(edLightYear);
    vl->addWidget(edAU);
    vl->addWidget(edPC);
    vl->addStretch();
    this->setLayout(vl);

    connect(edM,SIGNAL(valueChanged(double)),this,SLOT(changedM(double)));
    connect(edKM,SIGNAL(valueChanged(double)),this,SLOT(changedKM(double)));
    connect(edTTKM,SIGNAL(valueChanged(double)),this,SLOT(changedTTKM(double)));
    connect(edSND,SIGNAL(valueChanged(double)),this,SLOT(changedSND(double)));
    connect(edLightSec,SIGNAL(valueChanged(double)),this,SLOT(changedLS(double)));
    connect(edLightMin,SIGNAL(valueChanged(double)),this,SLOT(changedLM(double)));
    connect(edLightHour,SIGNAL(valueChanged(double)),this,SLOT(changedLH(double)));
    connect(edLightDay,SIGNAL(valueChanged(double)),this,SLOT(changedLD(double)));
    connect(edLightYear,SIGNAL(valueChanged(double)),this,SLOT(changedLY(double)));
    connect(edAU,SIGNAL(valueChanged(double)),this,SLOT(changedAU(double)));
    connect(edPC,SIGNAL(valueChanged(double)),this,SLOT(changedPC(double)));
    connect(edSEAMILE,SIGNAL(valueChanged(double)),this,SLOT(changedSM(double)));

    connect(edFeet,SIGNAL(valueChanged(double)),this,SLOT(changedFT(double)));
    connect(edInch,SIGNAL(valueChanged(double)),this,SLOT(changedIC(double)));
    connect(edYard,SIGNAL(valueChanged(double)),this,SLOT(changedYD(double)));
    connect(edMile,SIGNAL(valueChanged(double)),this,SLOT(changedML(double)));
}
//*******************************************
void DistanceConv::setCaption(QString s)
{
    lbCaption->setText(s);
    this->setWindowTitle(s);
}
//*******************************************
void DistanceConv::setValue(double d)
{
    edM->setValue(d);
}
//*******************************************
void DistanceConv::setReadOnly(bool b)
{
    edM->setReadOnly(b);
    edKM->setReadOnly(b);
    edTTKM->setReadOnly(b);
    edSND->setReadOnly(b);
    edLightSec->setReadOnly(b);
    edLightMin->setReadOnly(b);
    edLightHour->setReadOnly(b);
    edLightDay->setReadOnly(b);
    edLightYear->setReadOnly(b);
}

//*******************************************
void DistanceConv::changedM(double d)
{
    if (refFlag) return;
    refFlag = true;

    FsDistance fd;
    fd.setM(d);
    //edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());


    refFlag = false;
}
//*******************************************
void DistanceConv::changedKM(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setKm(d);
    edM->setValue(fd.M());
    //edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());

    refFlag = false;

}
//*******************************************
void DistanceConv::changedTTKM(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setTKm(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    //edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());

    refFlag = false;
}
//*******************************************
void DistanceConv::changedSND(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setSS(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    //edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());

    refFlag = false;
}
//*******************************************
void DistanceConv::changedSM(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setSEAMILE(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    //edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());

    refFlag = false;
}
//*******************************************
void DistanceConv::changedLS(double d)
{
    if (refFlag) return;
    FsDistance fd;
    fd.setLS(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    //edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());

    refFlag = false;
}
//*******************************************
void DistanceConv::changedLM(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setLM(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    //edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;
}
//*******************************************
void DistanceConv::changedLH(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setLH(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    //edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;
}
//*******************************************
void DistanceConv::changedLD(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setLD(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    //edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());

    refFlag = false;
}
//*******************************************
void DistanceConv::changedLY(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setLY(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    //edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;
}
//*******************************************
void DistanceConv::changedAU(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setAU(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    //edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;

}
//*******************************************
void DistanceConv::changedPC(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setPARSEC(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    //edPC->setValue(fd.PARSEC());
    refFlag = false;

}
//*******************************************
void DistanceConv::changedFT(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setFEET(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    //edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;
}

//*******************************************
void DistanceConv::changedIC(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setINCH(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    //edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;

}

//*******************************************
void DistanceConv::changedYD(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setYARD(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    //edYard->setValue(fd.YARD());
    edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;

}

//*******************************************
void DistanceConv::changedML(double d)
{
    if (refFlag) return;
    refFlag = true;
    FsDistance fd;
    fd.setMILE(d);
    edM->setValue(fd.M());
    edKM->setValue(fd.Km());
    edTTKM->setValue(fd.TKm());
    edSND->setValue(fd.SS());
    edSEAMILE->setValue(fd.SEAMILE());
    edFeet->setValue(fd.FEET());
    edInch->setValue(fd.INCH());
    edYard->setValue(fd.YARD());
    //edMile->setValue(fd.MILE());
    edLightSec->setValue(fd.LS());
    edLightMin->setValue(fd.LM());
    edLightHour->setValue(fd.LH());
    edLightDay->setValue(fd.LD());
    edLightYear->setValue(fd.LY());
    edAU->setValue(fd.AU());
    edPC->setValue(fd.PARSEC());
    refFlag = false;

}

//*******************************************
