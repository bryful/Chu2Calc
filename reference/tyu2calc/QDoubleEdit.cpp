#include "QDoubleEdit.h"

QDoubleEdit::QDoubleEdit(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    //this->setValidator(new QDoubleValidator(DBL_MIN,DBL_MAX, 10, this));
    this->setMinimum(0);
    this->setMaximum(DBL_MAX);
    this->setMinimumWidth(30);
    this->setMaximumWidth(1280);
    this->setDecimals(10);
    this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

    QFont f = this->font();
    f.setBold(true);
    f.setPointSize(12);
    this->setFont(f);
}

//***********************************************************
