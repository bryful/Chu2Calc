#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QDebug>
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>

#include <QStandardPaths>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRect>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QTextStream>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>
#include <QAction>

#include "FsSpace.h"

#include "AccelaTime.h"
#include "SpeedConv.h"
#include "Acceleration1.h"
#include "Acceleration2.h"
#include "FreeFallTime.h"
#include "FreeFallDistance.h"
#include "Satellite.h"
#include "DistanceConv.h"
#include "CentrifugalForce.h"



#include"../../Fs/FsPref.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTabWidget  *tab;

    void PrefSave();
    void PrefLoad();

public slots:
    SpeedConv* speedConvForm();
    DistanceConv* distanceConvForm();
    void currentSC();
    void currentDC();
    void currentAT();
    void currentA1();
    void currentA2();
    void currentST();
    void currentCF();
    void currentFT();
    void currentFD();

};

#endif // MAINWINDOW_H
