#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "Avantes.h"
#include <mutex>
//using namespace Avantes ;
extern bool dataReady ;
extern double specData [] ;

std::mutex specLock ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int status ;
    ui->setupUi(this);

    ui->autoIntButton->setStyleSheet ("background-color:orange") ;
    ui->darkButton->setStyleSheet ("background-color:yellow") ;
    //ui->stopCollectButton->setStyleSheet ("background-color:red") ;
    ui->stopCollectButton->setStyleSheet
        ("background-color:#a8ffff") ;
    ui->stopCollectButton->setEnabled (false) ;
    ui->singleScanButton->setStyleSheet("background-color:orange") ;
    ui->contButton->setStyleSheet("background-color:orange") ;

    gps = new GPS () ;
    gps->init() ;
    gps->start() ;
    ava = new Avaspec () ;
    status = ava->init() ;
//    specdata = new float [ava->npix * ava->nspecs] ;
     connect (ava, SIGNAL(gotData()), this, SLOT(on_gotData())) ;
//    connect (ava, SIGNAL(newInt(int)), this, SLOT (on_newInt(int))) ;

      if (status < 1){
          qDebug() << "No spectrometer found" ;
      }

      waves = new double [3648] ;
      specdata = new double [3648] ;
      for (int i=0; i<3648; i++) {
          waves[i] = ava->waves[i] ;
          specdata[i] = 0. ;
      }
      ui->specPlot->setAxisRange (0, ava->waves[0], ava->waves[ava->npix-1]) ;


      upTimer = new QTimer (this) ;
      connect (upTimer, SIGNAL (timeout()), this, SLOT(updateGUI())) ;
      upTimer->start(800) ;

}

MainWindow::~MainWindow()
{
    gps->stop() ;
    //delete gps ;
    delete ava ;
    delete ui;
    delete []waves ;
    delete [] specdata ;
    //avantes_closeup() ;
}

void MainWindow::on_singleScanButton_clicked()
{



    ava->setScanData (1000, specdata) ;
    ava->takeSingle() ;


//   ava->getSingleScan (data) ;
//   for (int i=0; i<ava->nspecs; i++)
//   ui->specPlot->setXYData (i, ava->waves, &data[i*ava->npix], ava->npix) ;

//   delete [] data ;

}



void MainWindow::on_gotData() {
     //for (int i=0; i<ava->nspecs; i++)
    ui->specPlot->setXYData (0, waves, specdata, 3648) ;
    qDebug() <<"data[2000] : " << specdata[2000]  ;

}

void MainWindow::on_newInt(int ival) {
    //QString str = QString ("%1").arg(ival) ;
    ui->integCB->setCurrentIndex (ival) ;

}


//void MainWindow::on_integTE_returnPressed()
//{
//    // get new integration time
//    float itime = ui->integTE->text().toFloat() ;
//    ava->setIntegrationTime (0, itime) ;
//}

void MainWindow::on_stopCollectButton_clicked()
{
    //ava->stop() ;
    ava->setContFlag (false ) ;
    ui->stopCollectButton->setEnabled(false);
    ui->stopCollectButton->setStyleSheet
        ("background-color:#a8ffff") ;
    ui->contButton->setStyleSheet("background-color:orange") ;
}

void MainWindow::on_autoIntButton_clicked()
{
    ava->setScanData (1000, specdata) ;
    ava->autoIntegrate (0) ;
    ui->autoIntButton->setStyleSheet("background-color:green") ;
}


void MainWindow::on_darkButton_clicked()
{
    //avantes_collectDark() ;
    //int status = QMessageBox::information (this, tr("Dark Cal..."), tr ("Prepare for dark then hit ok")) ;
    //ava->setScanData (1000, specdata) ;
    ava->setScanData (1000,specdata) ;
    ava->takeDark () ;
    ui->darkButton->setStyleSheet ("background-color:orange; color:white") ;
//
}


void MainWindow::updateGUI () {
//    if (dataReady) {
//        specLock.lock() ;
//        //ui->specPlot->setXYData (0, waves, specData, 3648) ;

//        //dataReady = false ;
//        qDebug() << "Plotting " << endl ;
//        specLock.unlock();
//    }
    if (gps) {
        QString qs = QString("%1 %2 %3").arg(gps->lat).arg(gps->lon).arg(gps->alt) ;
        ui->gpsOutLE->setText (qs) ;
        qDebug () << qs ;
    }
}

void MainWindow::on_contButton_clicked()
{
    ui->stopCollectButton->setEnabled (true) ;
    ui->stopCollectButton->setStyleSheet
        ("background-color:#ff3f3f") ;
    ui->contButton->setStyleSheet("background-color:green") ;
    ava->setContFlag (true) ;
    ava->setScanData(1000, specdata);
    ava->takeCont() ;
}
