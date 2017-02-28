#include "myplot.h"

MyPlot::MyPlot(QWidget *parent) :
    QCustomPlot(parent)
{

    nprofiles = 2 ;
    npts = 3648 ;
    addGraph () ;
    addGraph () ;
    yAxis->setRange (-200,600) ;
    xAxis->setRange (200,1200) ;
    QPen p (Qt::black) ;
    graph(0)->setPen (p) ;
    QPen p1 (Qt::magenta) ;
    graph(1)->setPen (p1) ;

    replot() ;



}

void MyPlot::setAxisRange (int ax, float min, float max) {
    switch (ax) {
        case 0:
            xAxis->setRange (min, max) ;
            break ;
        case 1 :
            yAxis->setRange (min, max) ;
            break ;
    }
}



void MyPlot::setXYData (int igraph, float *xdata, float *ydata, int npts) {

    int i = 0 ;
    xVec.clear() ;
    yVec.clear() ;
    for (i=0; i<npts; i++){
        xVec.append(xdata[i]) ;
        yVec.append(ydata[i]) ;
    }
    this->graph(igraph)->setData (xVec, yVec) ;
    this->rescaleAxes() ;
    this->replot() ;
qDebug () << "replotting" ;

}


void MyPlot::setXYData (int igraph, double *xdata, double *ydata, int npts) {

    int i = 0 ;
    xVec.clear() ;
    yVec.clear() ;
    for (i=0; i<npts; i++){
        xVec.append(xdata[i]) ;
        yVec.append(ydata[i]) ;
    }
    this->graph(igraph)->setData (xVec, yVec) ;
    this->rescaleAxes() ;
    this->replot() ;
qDebug () << "replotting" ;

}

