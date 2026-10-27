#include "QAccelerEdit.h"

QAccelerEdit::QAccelerEdit(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    m_value.setMPS2(0);
    lbCaption = new QLabel(FL8("加速度"));
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cmbUnit = new QComboBox;
    cmbUnit->addItem(A_G);
    cmbUnit->addItem(A_MPS2);
    cmbUnit->addItem(A_KMPS2);
    cmbUnit->addItem(A_TKMPS2);

    cmbUnit->addItem(A_MPM2);
    cmbUnit->addItem(A_KMPM2);
    cmbUnit->addItem(A_TKMPM2);

    cmbUnit->addItem(A_MPH2);
    cmbUnit->addItem(A_KMPH2);
    cmbUnit->addItem(A_TKMPH2);

    edBox = new QDoubleSpinBox;
    edBox->setMinimum(-DBL_MAX);
    edBox->setMaximum(DBL_MAX);
    edBox->setMinimumWidth(30);
    edBox->setMaximumWidth(1280);
    edBox->setDecimals(12);
    edBox->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    QFont f = edBox->font();
    f.setBold(true);
    f.setPointSize(12);
    edBox->setFont(f);




    QVBoxLayout *vl = new QVBoxLayout;
    QHBoxLayout *hl = new QHBoxLayout;
    hl->setSpacing(0);
    hl->addWidget(lbCaption);
    hl->addStretch();
    hl->addWidget(cmbUnit);
    hl->addSpacing(5);
    vl->setSpacing(1);
    vl->addLayout(hl);
    vl->addWidget(edBox);
    vl->setMargin(0);
    this->setLayout(vl);

    connect(edBox,SIGNAL(valueChanged(double)),this,SLOT(changedBox(double)));
    connect(cmbUnit,SIGNAL(currentIndexChanged(int)),this,SLOT(setMode(int)));

    cmbUnit->setCurrentIndex(MS2);
    setMode(MS2);
}
//**************************************************
void QAccelerEdit::setReadOnly(bool b)
{
    edBox->setReadOnly(b);
}
//**************************************************
void QAccelerEdit::setCaption(QString s)
{
    lbCaption->setText(s);
}

//**************************************************
void QAccelerEdit::setValue(double d)
{
    m_value.setMPS2(d);
    dispValue();
}
//**************************************************
void QAccelerEdit::dispValue()
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    double dd;
    switch (md){
    case G:
        dd = m_value.G();
        break;

    case MS2:
        dd = m_value.MPS2();
        break;
    case KS2:
        dd = m_value.KMPS2();
        break;
    case TKS2:
        dd = m_value.TKMPS2();
        break;
    case MM2:
        dd = m_value.MPM2();
        break;
    case KM2:
        dd = m_value.KMPM2();
        break;
    case TKM2:
        dd = m_value.TKMPM2();
        break;
    case MH2:
        dd = m_value.MPH2();
        break;
    case KH2:
        dd = m_value.KMPH2();
        break;
    case TKH2:
        dd = m_value.TKMPH2();
        break;
    }
    if (edBox->value() != dd){
        edBox->setValue(dd);
    }
    refFlag = false;
}
//**************************************************
void QAccelerEdit::changedBox(double d)
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    bool dd = false;
    switch (md){
    case G:
        dd = m_value.setG(d);
        break;
    case MS2:
        dd = m_value.setMPS2(d);
        break;
    case KS2:
        dd = m_value.setKMPS2(d);
        break;
    case TKS2:
        dd = m_value.setTKMPS2(d);
        break;
    case MM2:
        dd = m_value.setMPM2(d);
        break;
    case KM2:
        dd = m_value.setKMPM2(d);
        break;
    case TKM2:
        dd = m_value.setTKMPM2(d);
        break;
    case MH2:
        dd = m_value.setMPH2(d);
        break;
    case KH2:
        dd = m_value.setKMPH2(d);
        break;
    case TKH2:
        dd = m_value.setTKMPH2(d);
        break;
    }
    if (dd) {
        changedValue(m_value.MPS2());
    }
    refFlag = false;
}
//**************************************************
void QAccelerEdit::setMode(int md)
{

    cmbUnit->setCurrentIndex(md);
    switch (md){
    case G:
        edBox->setSuffix(" " + A_G);
        break;
    case MS2:
        edBox->setSuffix(" "+A_MPS2);
        break;
    case KS2:
        edBox->setSuffix(" " + A_KMPS2);
        break;
    case TKS2:
        edBox->setSuffix(" " + A_TKMPS2);
        break;
    case MM2:
        edBox->setSuffix(" "+A_MPM2);
        break;
    case KM2:
        edBox->setSuffix(" " + A_KMPM2);
        break;
    case TKM2:
        edBox->setSuffix(" " + A_TKMPM2);
        break;
    case MH2:
        edBox->setSuffix(" "+A_MPH2);
        break;
    case KH2:
        edBox->setSuffix(" " + A_KMPH2);
        break;
    case TKH2:
        edBox->setSuffix(" " + A_TKMPH2);
        break;
    }
    dispValue();
}
//**************************************************

