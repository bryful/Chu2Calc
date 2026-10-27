#include "QSpeedEdit.h"

QSpeedEdit::QSpeedEdit(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    m_value.setMPS(0);

    lbCaption = new QLabel(FL8("速度"));
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cmbUnit = new QComboBox;
    cmbUnit->addItem(S_MPS);
    cmbUnit->addItem(S_KMPS);
    cmbUnit->addItem(S_MPM);
    cmbUnit->addItem(S_KMPM);
    cmbUnit->addItem(S_MPH);
    cmbUnit->addItem(S_KMPH);
    cmbUnit->addItem(S_TKMPH);
    cmbUnit->addItem(S_MACH);
    cmbUnit->addItem(S_KNT);
    cmbUnit->addItem(S_LPS);
    cmbUnit->addItem(S_LPM);
    cmbUnit->addItem(S_LPH);
    cmbUnit->addItem(S_LPD);
    cmbUnit->addItem(S_LPY);
    cmbUnit->setCurrentIndex(MS);

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
    hl->addWidget(lbCaption);
    hl->addStretch();
    hl->addWidget(cmbUnit);
    hl->addSpacing(5);
    vl->setSpacing(0);
    vl->addLayout(hl);
    vl->addWidget(edBox);
    vl->setMargin(0);
    this->setLayout(vl);

    connect(edBox,SIGNAL(valueChanged(double)),this,SLOT(changedBox(double)));
    connect(cmbUnit,SIGNAL(currentIndexChanged(int)),this,SLOT(setMode(int)));
    cmbUnit->setCurrentIndex(MS);
    setMode(MS);

}
//**************************************************
void QSpeedEdit::setReadOnly(bool b)
{
    edBox->setReadOnly(b);
}
//**************************************************
void QSpeedEdit::setCaption(QString s)
{
    lbCaption->setText(s);
}

//**************************************************
void QSpeedEdit::dispValue()
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    double dd;
    switch (md){
    case MS:
        dd = m_value.MPS();
        break;
    case KS:
        dd = m_value.KMPS();
        break;
    case MM:
        dd = m_value.MPM();
        break;
    case KM:
        dd = m_value.KMPM();
        break;
    case MH:
        dd = m_value.MPH();
        break;
    case KH:
        dd = m_value.KMPH();
        break;
    case TTKH:
        dd = m_value.TKMPH();
        break;
    case MACH:
        dd = m_value.MACH();
        break;
    case KNOT:
        dd = m_value.KNOT();
        break;
    case LS:
        dd = m_value.LS();
        break;
    case LM:
        dd = m_value.LM();
        break;
    case LH:
        dd = m_value.LH();
        break;
    case LD:
        dd = m_value.LD();
        break;
    case LY:
        dd = m_value.LY();
        break;
    }

    if (edBox->value() != dd){
        edBox->setValue(dd);
    }
    refFlag = false;
}
//**************************************************
void QSpeedEdit::changedBox(double d)
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    bool b = false;
    switch (md){
    case MS:
        b = m_value.setMPS(d);
        break;
    case KS:
        b = m_value.setKMPS(d);
        break;
    case MM:
        b = m_value.setMPM(d);
        break;
    case KM:
        b = m_value.setKMPM(d);
        break;
    case MH:
        b = m_value.setMPH(d);
        break;
    case KH:
        b = m_value.setKMPH(d);
        break;
    case TTKH:
        b = m_value.setTKMPH(d);
        break;
    case MACH:
        b = m_value.setMACH(d);
        break;
    case KNOT:
        b = m_value.setKNOT(d);
        break;
    case LS:
        b = m_value.setLS(d);
        break;
    case LM:
        b = m_value.setLM(d);
        break;
    case LH:
        b = m_value.setLH(d);
        break;
    case LD:
        b = m_value.setLD(d);
        break;
    case LY:
        b = m_value.setLY(d);
        break;
    }
    if (b) {
        changedValue(m_value.MPS());
    }
    refFlag = false;

}
//**************************************************
void QSpeedEdit::setMode(int md)
{

    cmbUnit->setCurrentIndex(md);
    switch (md){
    case MS:
        edBox->setSuffix(" "+ S_MPS);
        break;
    case KS:
        edBox->setSuffix(" "+ S_KMPS);
        break;
    case MM:
        edBox->setSuffix(" "+ S_MPM);
        break;
    case KM:
        edBox->setSuffix(" " + S_KMPM);
        break;
    case MH:
        edBox->setSuffix(" " + S_MPH);
        break;
    case KH:
        edBox->setSuffix(" " + S_KMPH);
        break;
    case TTKH:
        edBox->setSuffix(" " + S_TKMPH);
        break;
    case MACH:
        edBox->setSuffix(" " + S_MACH);
        break;
    case KNOT:
        edBox->setSuffix(" " + S_KNT);
        break;
    case LS:
        edBox->setSuffix(" " + S_LPS);
        break;
    case LM:
        edBox->setSuffix(" " + S_LPM);
        break;
    case LH:
        edBox->setSuffix(" " + S_LPH);
        break;
    case LD:
        edBox->setSuffix(" " + S_LPD);
        break;
    case LY:
        edBox->setSuffix(" " + S_LPD);
        break;
    }
    dispValue();

}
//**************************************************
