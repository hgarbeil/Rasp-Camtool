#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Avaspec.h"
#include "GPS.h"
#include "phidgetsMot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Avaspec *ava ;
    phidgetsMot *pm ;
    GPS *gps ;
    int numSpec ;

    double *waves, *specdata ;
    QTimer *upTimer ;
private slots:
    void on_singleScanButton_clicked();
    void on_gotData() ;
    void plotSpec (int) ;
    void on_newInt (int) ;
    void updateGUI () ;


//    void on_integTE_returnPressed();

    void on_stopCollectButton_clicked();

    void on_autoIntButton_clicked();

    void on_darkButton_clicked();

    void on_contButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
