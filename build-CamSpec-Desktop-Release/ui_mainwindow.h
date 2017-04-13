/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "myplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    MyPlot *specPlot;
    QPushButton *singleScanButton;
    QPushButton *stopCollectButton;
    QPushButton *darkButton;
    QPushButton *autoIntButton;
    QComboBox *integCB;
    QPushButton *contButton;
    QLineEdit *gpsOutLE;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(703, 387);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        specPlot = new MyPlot(centralWidget);
        specPlot->setObjectName(QStringLiteral("specPlot"));
        specPlot->setGeometry(QRect(150, 0, 521, 271));
        singleScanButton = new QPushButton(centralWidget);
        singleScanButton->setObjectName(QStringLiteral("singleScanButton"));
        singleScanButton->setGeometry(QRect(20, 130, 111, 31));
        stopCollectButton = new QPushButton(centralWidget);
        stopCollectButton->setObjectName(QStringLiteral("stopCollectButton"));
        stopCollectButton->setGeometry(QRect(20, 240, 121, 31));
        darkButton = new QPushButton(centralWidget);
        darkButton->setObjectName(QStringLiteral("darkButton"));
        darkButton->setGeometry(QRect(20, 90, 111, 31));
        autoIntButton = new QPushButton(centralWidget);
        autoIntButton->setObjectName(QStringLiteral("autoIntButton"));
        autoIntButton->setGeometry(QRect(20, 50, 111, 31));
        integCB = new QComboBox(centralWidget);
        integCB->setObjectName(QStringLiteral("integCB"));
        integCB->setGeometry(QRect(20, 10, 111, 31));
        contButton = new QPushButton(centralWidget);
        contButton->setObjectName(QStringLiteral("contButton"));
        contButton->setGeometry(QRect(20, 170, 111, 31));
        gpsOutLE = new QLineEdit(centralWidget);
        gpsOutLE->setObjectName(QStringLiteral("gpsOutLE"));
        gpsOutLE->setGeometry(QRect(150, 280, 521, 33));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 703, 27));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        integCB->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        singleScanButton->setText(QApplication::translate("MainWindow", "Take Single", 0));
        stopCollectButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        darkButton->setText(QApplication::translate("MainWindow", "Collect Dark", 0));
        autoIntButton->setText(QApplication::translate("MainWindow", "Auto Integrate", 0));
        integCB->clear();
        integCB->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "50", 0)
         << QApplication::translate("MainWindow", "100", 0)
         << QApplication::translate("MainWindow", "250", 0)
         << QApplication::translate("MainWindow", "500", 0)
         << QApplication::translate("MainWindow", "750", 0)
         << QApplication::translate("MainWindow", "1000", 0)
         << QApplication::translate("MainWindow", "1500", 0)
        );
        contButton->setText(QApplication::translate("MainWindow", "Continuous", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
