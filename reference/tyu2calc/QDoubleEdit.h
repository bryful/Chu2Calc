#ifndef QDOUBLEEDIT_H
#define QDOUBLEEDIT_H

#include <float.h>

#include <QSize>

#include <QDoubleSpinBox>
#include <QKeyEvent>

#include <QtCore/qmath.h>

#include <QValidator>
#include <QDoubleValidator>

#include "FsSpace.h"

class QDoubleEdit : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit QDoubleEdit(QWidget *parent = 0);

signals:

public slots:


protected:

private:

};

#endif // QDOUBLEEDIT_H
