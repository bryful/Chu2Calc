#include "QWeightEdit.h"

QWeightEdit::QWeightEdit(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    m_value = 0;
    lbCaption = new QLabel(FL8("重量"));
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cmbUnit = new QComboBox;
    cmbUnit->addItem(FL8("g"));
    cmbUnit->addItem(FL8("kg"));
    cmbUnit->addItem(FL8("t"));
    cmbUnit->addItem(FL8("万t"));

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

    cmbUnit->setCurrentIndex(G);
    setMode(G);
}
//**************************************************
void QWeightEdit::setReadOnly(bool b)
{
    edBox->setReadOnly(b);
}
//**************************************************
void QWeightEdit::setCaption(QString s)
{
    lbCaption->setText(s);
}

//**************************************************
void QWeightEdit::setValue(double d)
{
    m_value = d;
    dispValue();
}
//**************************************************
void QWeightEdit::dispValue()
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    double dd;
    switch (md){
    case G:
        dd = m_value;
        break;
    case KG:
        dd = m_value/1000;
        break;
    case T:
        dd = m_value/(1000*1000);
        break;
    case TTT:
        dd = m_value/(1000*1000);
        dd /= 10000;
        break;
    }
    if (edBox->value() != dd){
        edBox->setValue(dd);
    }
    refFlag = false;
}
//**************************************************
void QWeightEdit::changedBox(double d)
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    double dd = 0;
    switch (md){
    case G:
        dd = d;
        break;
    case KG:
        dd = d * 1000;
        break;
    case T:
        dd = d * 1000* 1000;
        break;
    case TTT:
        dd = d*1000*1000*10000;
        break;
    }
    if (dd != m_value) {
        m_value = dd;
        changedValue(m_value);
    }
    refFlag = false;
}
//**************************************************
void QWeightEdit::setMode(int md)
{

    cmbUnit->setCurrentIndex(md);
    switch (md){
    case G:
        edBox->setSuffix(" " +FL8("g"));
        break;
    case KG:
        edBox->setSuffix(" " +FL8("kg"));
        break;
    case T:
        edBox->setSuffix(" " +FL8("t"));
        break;
    case TTT:
        edBox->setSuffix(" " +FL8("万t"));
        break;
    }
    dispValue();
}
//**************************************************

