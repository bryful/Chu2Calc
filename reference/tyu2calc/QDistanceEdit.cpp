#include "QDistanceEdit.h"

QDistanceEdit::QDistanceEdit(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    m_value.setM(0);
    lbCaption = new QLabel(FL8("距離"));
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cmbUnit = new QComboBox;
    cmbUnit->addItem(D_M);
    cmbUnit->addItem(D_KM);
    cmbUnit->addItem(D_TKM);
    cmbUnit->addItem(D_SS);

    cmbUnit->addItem(D_SM);
    cmbUnit->addItem(D_FT);
    cmbUnit->addItem(D_IC);
    cmbUnit->addItem(D_YD);
    cmbUnit->addItem(D_ML);

    cmbUnit->addItem(D_LS);
    cmbUnit->addItem(D_LM);
    cmbUnit->addItem(D_LH);
    cmbUnit->addItem(D_LD);
    cmbUnit->addItem(D_LY);
    cmbUnit->addItem(D_AU);
    cmbUnit->addItem(D_PC);

    edBox = new QDoubleSpinBox;
    edBox->setMinimum(0);
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
    cmbUnit->setCurrentIndex(M);
    setMode(M);
}
//**************************************************
void QDistanceEdit::setReadOnly(bool b)
{
    edBox->setReadOnly(b);
}
//**************************************************
void QDistanceEdit::setCaption(QString s)
{
    lbCaption->setText(s);
}

//**************************************************
void QDistanceEdit::setValue(double d)
{
    m_value.setM(d);
    dispValue();
}
//**************************************************
void QDistanceEdit::dispValue()
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    double b = 0;
    switch (md){
    case M:
        b = m_value.M();
        break;
    case K:
        b = m_value.Km();
        break;
    case TK:
        b = m_value.TKm();
        break;
    case SS:
        b = m_value.SS();
        break;
    case SEAMILE:
        b = m_value.SEAMILE();
        break;
    case FEET:
        b = m_value.FEET();
        break;
    case INCH:
        b = m_value.INCH();
        break;
    case YARD:
        b = m_value.YARD();
        break;
    case MILE:
        b = m_value.MILE();
        break;
    case LS:
        b = m_value.LS();
        break;
    case LM:
        b = m_value.LM();
        break;
    case LH:
        b = m_value.LH();
        break;
    case LD:
        b = m_value.LD();
        break;
    case LY:
        b = m_value.LY();
        break;
    case AU:
        b = m_value.AU();
        break;
    case PC:
        b = m_value.PARSEC();
        break;
    }

    if (edBox->value() != b){
        edBox->setValue(b);
    }
    refFlag = false;
}
//**************************************************
void QDistanceEdit::changedBox(double d)
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    bool b = false;
    switch (md){
    case M:
        b = m_value.setM(d);
        break;
    case K:
        b = m_value.setKm(d);
        break;
    case TK:
        b = m_value.setTKm(d);
        break;
    case SS:
        b = m_value.setSS(d);
        break;
    case SEAMILE:
        b = m_value.setSEAMILE(d);
        break;
    case FEET:
        b = m_value.setFEET(d);
        break;
    case INCH:
        b = m_value.setINCH(d);
        break;
    case YARD:
        b = m_value.setYARD(d);
        break;
    case MILE:
        b = m_value.setMILE(d);
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
    case AU:
        b = m_value.setAU(d);
        break;
    case PC:
        b = m_value.setPARSEC(d);
        break;
    }
    if (b) {
        changedValue(m_value.M());
    }
    refFlag = false;

}
//**************************************************
void QDistanceEdit::setMode(int md)
{
    cmbUnit->setCurrentIndex(md);
    switch (md){
    case M:
        edBox->setSuffix(" " + D_M);
        break;
    case K:
        edBox->setSuffix(" " + D_KM);
        break;
    case TK:
        edBox->setSuffix(" " + D_TKM);
        break;
    case SS:
        edBox->setSuffix(" " + D_SS);
        break;
    case SEAMILE:
        edBox->setSuffix(" " + D_SM);
        break;
    case FEET:
        edBox->setSuffix(" " + D_FT);
        break;
    case INCH:
        edBox->setSuffix(" " + D_IC);
        break;
    case YARD:
        edBox->setSuffix(" " + D_YD);
        break;
    case MILE:
        edBox->setSuffix(" " + D_ML);
        break;
    case LS:
        edBox->setSuffix(" " + D_LS);
        break;
    case LM:
        edBox->setSuffix(" " + D_LM);
        break;
    case LH:
        edBox->setSuffix(" " + D_LH);
        break;
    case LD:
        edBox->setSuffix(" " + D_LD);
        break;
    case LY:
        edBox->setSuffix(" " + D_LY);
        break;
    case AU:
        edBox->setSuffix(" " + D_AU);
        break;
    case PC:
        edBox->setSuffix(" " + D_PC);
        break;
    }
    dispValue();

}
//**************************************************
