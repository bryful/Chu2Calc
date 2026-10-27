#include "QTimeValueEdit.h"

QTimeValueEdit::QTimeValueEdit(QWidget *parent) :
    QWidget(parent)
{
    refFlag = false;
    m_value = 0;
    lbCaption = new QLabel(FL8("時間"));
    lbCaption->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cmbUnit = new QComboBox;
    cmbUnit->addItem(T_SEC);
    cmbUnit->addItem(T_MIN);
    cmbUnit->addItem(T_HOUR);
    cmbUnit->addItem(T_DAY);
    cmbUnit->addItem(T_YEAR);
    cmbUnit->addItem(T_YDHMS);
    cmbUnit->setCurrentIndex(SEC);

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

    edYear = mkBox();
    edDay = mkBox();
    edHour = mkBox(500);
    edMin = mkBox(500);
    edSec = mkBox(500);
    edMSec = new QDoubleSpinBox;
    edMSec->setMaximumWidth(400);
    edMSec->setMinimumWidth(80);
    edMSec->setMinimum(0);
    edMSec->setMaximum(1.0);
    edMSec->setDecimals(5);
    edMSec->setSingleStep(0.01);
    edMSec->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    f = edMSec->font();
    f.setBold(true);
    f.setPointSize(12);
    edMSec->setFont(f);

    lbY = new QLabel(T_YEAR);
    lbD = new QLabel(T_DAY);
    lbH = new QLabel(T_HOUR);
    lbM = new QLabel(T_MIN);
    lbS = new QLabel(T_SEC);
    lbMS = new QLabel(".");
    lbY->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lbD->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lbH->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lbM->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lbS->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lbMS->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);




    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(lbCaption);
    hl->addStretch();
    hl->addWidget(cmbUnit);
    hl->addSpacing(5);

    hlYDHMS = new QHBoxLayout;
    hlYDHMS->setSpacing(3);
    hlYDHMS->addWidget(edYear);
    hlYDHMS->addWidget(lbY);
    hlYDHMS->addWidget(edDay);
    hlYDHMS->addWidget(lbD);
    hlYDHMS->addWidget(edHour);
    hlYDHMS->addWidget(lbH);
    hlYDHMS->addWidget(edMin);
    hlYDHMS->addWidget(lbM);
    hlYDHMS->addWidget(edSec);
    hlYDHMS->addWidget(lbMS);
    hlYDHMS->addWidget(edMSec);
    hlYDHMS->addWidget(lbS);
    //hlYDHMS->addStretch();

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setSpacing(0);
    vl->setMargin(0);
    vl->addLayout(hl);
    vl->addWidget(edBox);
    vl->addLayout(hlYDHMS);
    this->setLayout(vl);

    connect(edBox,SIGNAL(valueChanged(double)),this,SLOT(changedBox(double)));
    connect(cmbUnit,SIGNAL(currentIndexChanged(int)),this,SLOT(setMode(int)));

    connect(edYear,SIGNAL(valueChanged(int)),this,SLOT(changedBoxYEAR(int)));
    connect(edDay,SIGNAL(valueChanged(int)),this,SLOT(changedBoxDAY(int)));
    connect(edHour,SIGNAL(valueChanged(int)),this,SLOT(changedBoxHOUR(int)));
    connect(edMin,SIGNAL(valueChanged(int)),this,SLOT(changedBoxMIN(int)));
    connect(edSec,SIGNAL(valueChanged(int)),this,SLOT(changedBoxSEC(int)));
    connect(edMSec,SIGNAL(valueChanged(double)),this,SLOT(changedBoxMSEC(double)));
    cmbUnit->setCurrentIndex(SEC);
    setMode(SEC);

}
//**************************************************
QSpinBox* QTimeValueEdit::mkBox(int v)
{
    QSpinBox *box;
    box = new QSpinBox;
    box->setMaximumWidth(v);
    box->setMinimumWidth(10);
    box->setMinimum(0);
    box->setMaximum(99999);
    box->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    QFont f = box->font();
    f.setBold(true);
    f.setPointSize(12);
    box->setFont(f);
    return box;

}

//**************************************************
void QTimeValueEdit::setReadOnly(bool b)
{
    edBox->setReadOnly(b);
}
//**************************************************
void QTimeValueEdit::setCaption(QString s)
{
    lbCaption->setText(s);
}

//**************************************************
void QTimeValueEdit::setValue(double d)
{
    m_value.setSEC(d);
    dispValue();
}
//**************************************************
void QTimeValueEdit::dispValue()
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    if (md ==YDHMS){
     if (edYear->value() != m_value.year()) edYear->setValue(m_value.year());
     if (edDay->value() != m_value.day()) edDay->setValue(m_value.day());
     if (edHour->value() != m_value.hour()) edHour->setValue(m_value.hour());
     if (edMin->value() != m_value.min()) edMin->setValue(m_value.min());
     if (edSec->value() != m_value.sec()) edSec->setValue(m_value.sec());
     if (edMSec->value() != m_value.microSec()) edMSec->setValue(m_value.microSec());

    }else{
        double dd;
        switch (md){
        case SEC:
            dd = m_value.SEC();
            break;
        case MIN:
            dd = m_value.MIN();
            break;
        case HOUR:
            dd = m_value.HOUR();
            break;
        case DAY:
            dd = m_value.DAY();
            break;
        case YEAR:
            dd = m_value.YEAR();
            break;
        }
        if (edBox->value() != dd){
            edBox->setValue(dd);
        }
    }
    refFlag = false;
}
//**************************************************
void QTimeValueEdit::changedBox(double d)
{
    if (refFlag) return;
    refFlag = true;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS){
        qDebug() <<"QTimeValueEdit::changedBox";
        bool b = false;
        switch (md){
        case SEC:
            b = m_value.setSEC(d);
            break;
        case MIN:
            b = m_value.setMIN(d);
            break;
        case HOUR:
            b = m_value.setHOUR(d);
            break;
        case DAY:
            b = m_value.setDAY(d);
            break;
        case YEAR:
            b = m_value.setYEAR(d);
            break;
        }
        if (b) {
            changedValue(m_value.SEC());
            qDebug() <<"OK";
        }
    }
    refFlag = false;
}
//**************************************************
void QTimeValueEdit::changedBoxYEAR(int v)
{
    if (refFlag) return;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS) return;
    qDebug() <<"QTimeValueEdit::changedBoxYEAR";
    refFlag = true;
    bool b = m_value.setYearP(v);
    if (b){
        changedValue(m_value.SEC());
        qDebug() <<"OK";
    }
    refFlag = false;
}

//**************************************************
void QTimeValueEdit::changedBoxDAY(int v)
{
    if (refFlag) return;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS) return;
    qDebug() <<"QTimeValueEdit::changedBoxDAY";
    refFlag = true;
    bool b = m_value.setDayP(v);
    if (b){
        changedValue(m_value.SEC());
        qDebug() <<"OK";
    }
    refFlag = false;
}

//**************************************************
void QTimeValueEdit::changedBoxHOUR(int v)
{
    if (refFlag) return;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS) return;
    qDebug() <<"QTimeValueEdit::changedBoxHOUR";
    refFlag = true;
    bool b = m_value.setHourP(v);
    if (b){
        changedValue(m_value.SEC());
        qDebug() <<"OK";
    }
    refFlag = false;

}

//**************************************************
void QTimeValueEdit::changedBoxMIN(int v)
{
    if (refFlag) return;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS) return;
    qDebug() <<"QTimeValueEdit::changedBoxMIN";
    refFlag = true;
    bool b = m_value.setMinP(v);
    if (b){
        changedValue(m_value.SEC());
        qDebug() <<"OK";
    }
    refFlag = false;

}

//**************************************************
void QTimeValueEdit::changedBoxSEC(int v)
{
    if (refFlag) return;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS) return;
    qDebug() <<"QTimeValueEdit::changedBoxSEC";
    refFlag = true;
    bool b = m_value.setSecP(v);
    if (b){
        changedValue(m_value.SEC());
        qDebug() <<"OK";
    }else{
        qDebug() << v;

    }
    refFlag = false;

}

//**************************************************
void QTimeValueEdit::changedBoxMSEC(double v)
{
    if (refFlag) return;
    int md = cmbUnit->currentIndex();
    if (md!=YDHMS) return;
    qDebug() <<"QTimeValueEdit::changedBoxMSEC";
    refFlag = true;
    bool b = m_value.setMicroSecP(v);
    if (b){
        changedValue(m_value.SEC());
        qDebug() <<"OK";
    }
    refFlag = false;

}

//**************************************************
void QTimeValueEdit::changeVisibled(bool b)
{
    edBox->setVisible(b);
    bool b2 = ! b;
    edYear->setVisible(b2);
    edDay->setVisible(b2);
    edHour->setVisible(b2);
    edMin->setVisible(b2);
    edSec->setVisible(b2);
    edMSec->setVisible(b2);
    lbY->setVisible(b2);
    lbD->setVisible(b2);
    lbH->setVisible(b2);
    lbM->setVisible(b2);
    lbS->setVisible(b2);
    lbMS->setVisible(b2);
}

//**************************************************
void QTimeValueEdit::setMode(int md)
{

    cmbUnit->setCurrentIndex(md);
    if (md == YDHMS){
        changeVisibled(false);
    }else{
        changeVisibled(true);
        switch (md){
        case SEC:
            edBox->setSuffix(" " +T_SEC);
            break;
        case MIN:
            edBox->setSuffix(" " +T_MIN);
            break;
        case HOUR:
            edBox->setSuffix(" " +T_HOUR);
            break;
        case DAY:
            edBox->setSuffix(" " +T_DAY);
            break;
        case YEAR:
            edBox->setSuffix(" " +T_YEAR);
            break;
        }
    }
    dispValue();
}
//**************************************************

