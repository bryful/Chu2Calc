#include "MainWindow.h"

//*********************************************************************
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(FL8("中２電卓"));
    SpeedConv *sc = new SpeedConv;
    DistanceConv *dc = new DistanceConv;
    AccelaTime *at = new AccelaTime;
    Acceleration1 *a1 = new Acceleration1;
    Acceleration2 *a2 = new Acceleration2;
    Satellite *st = new Satellite;
    CentrifugalForce *cf = new CentrifugalForce;
    FreeFallTime *ft = new FreeFallTime;
    FreeFallDistance *fd = new FreeFallDistance;


    tab = new QTabWidget;
    tab->addTab(sc, sc->windowTitle());
    tab->addTab(dc, dc->windowTitle());
    tab->addTab(at, at->windowTitle());
    tab->addTab(a1, a1->windowTitle());
    tab->addTab(a2, a2->windowTitle());
    tab->addTab(st, st->windowTitle());
    tab->addTab(cf, cf->windowTitle());
    tab->addTab(ft, ft->windowTitle());
    tab->addTab(fd, fd->windowTitle());



    QPushButton *btnSC = new QPushButton(FL8("速度変換"));
    QPushButton *btnDC = new QPushButton(FL8("距離変換"));
    QPushButton *btnAT = new QPushButton(FL8("加速時間から速度距離"));
    QPushButton *btnA1 = new QPushButton(FL8("速度時間から加速度"));
    QPushButton *btnA2 = new QPushButton(FL8("速度距離から加速度"));
    QPushButton *btnST = new QPushButton(FL8("人工衛星"));
    QPushButton *btnCF = new QPushButton(FL8("遠心重力"));
    QPushButton *btnFT = new QPushButton(FL8("自由落下(時間)"));
    QPushButton *btnFD = new QPushButton(FL8("自由落下(距離)"));

    btnSC->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    btnDC->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    btnAT->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(btnSC);
    vl->addWidget(btnDC);
    vl->addWidget(btnAT);
    vl->addWidget(btnA1);
    vl->addWidget(btnA2);
    vl->addWidget(btnST);
    vl->addWidget(btnCF);
    vl->addWidget(btnFT);
    vl->addWidget(btnFD);
    vl->addStretch();

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addLayout(vl);
    hl->addWidget(tab);


    QWidget *cw = new QWidget;
    cw->setLayout(hl);

    setCentralWidget(cw);

    PrefLoad();

    connect(btnSC,SIGNAL(clicked()),this,SLOT(currentSC()));
    connect(btnDC,SIGNAL(clicked()),this,SLOT(currentDC()));
    connect(btnAT,SIGNAL(clicked()),this,SLOT(currentAT()));
    connect(btnA1,SIGNAL(clicked()),this,SLOT(currentA1()));
    connect(btnA2,SIGNAL(clicked()),this,SLOT(currentA2()));
    connect(btnST,SIGNAL(clicked()),this,SLOT(currentST()));
    connect(btnCF,SIGNAL(clicked()),this,SLOT(currentCF()));
    connect(btnFT,SIGNAL(clicked()),this,SLOT(currentFT()));
    connect(btnFD,SIGNAL(clicked()),this,SLOT(currentFD()));


}

//*********************************************************************
MainWindow::~MainWindow()
{
    PrefSave();
}
//*********************************************************************
void MainWindow::PrefSave()
{
    FsPref pref(QApplication::applicationName());
    pref.setValueRect("geo",this->geometry());

    pref.save();
}
//*********************************************************************
void MainWindow::PrefLoad()
{
    FsPref pref(QApplication::applicationName());
    pref.load();
    bool ok;
    QRect r = pref.valueRect("geo",&ok);
    if(!ok){
        r.setLeft(100);
        r.setTop(200);
        r.setWidth(512);
        r.setHeight(540);

    }
    this->setGeometry(r);
}
//*********************************************************************
SpeedConv* MainWindow::speedConvForm()
{
    SpeedConv *dlg = new SpeedConv;
    dlg->show();
    QRect r = dlg->geometry();
    r.setWidth(250);
    dlg->setGeometry(r);
    return dlg;
}

//*********************************************************************
DistanceConv* MainWindow::distanceConvForm()
{
    DistanceConv *dlg = new DistanceConv;
    dlg->show();
    QRect r = dlg->geometry();
    r.setWidth(250);
    dlg->setGeometry(r);
    return dlg;
}

//*********************************************************************
void MainWindow::currentSC()
{
    tab->setCurrentIndex(0);
}
//*********************************************************************
void MainWindow::currentDC()
{
    tab->setCurrentIndex(1);
}
//*********************************************************************
void MainWindow::currentAT()
{
    tab->setCurrentIndex(2);
}
//*********************************************************************
void MainWindow::currentA1()
{
    tab->setCurrentIndex(3);
}
//*********************************************************************
void MainWindow::currentA2()
{
    tab->setCurrentIndex(4);
}
//*********************************************************************
void MainWindow::currentST()
{
    tab->setCurrentIndex(5);
}
//*********************************************************************
void MainWindow::currentCF()
{
    tab->setCurrentIndex(6);
}
//*********************************************************************
void MainWindow::currentFT()
{
    tab->setCurrentIndex(7);
}
//*********************************************************************
void MainWindow::currentFD()
{
    tab->setCurrentIndex(8);
}
