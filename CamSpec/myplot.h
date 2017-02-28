#ifndef MYPLOT_H
#define MYPLOT_H

#include <qcustomplot.h>

class MyPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit MyPlot(QWidget *parent = 0);
    int nprofiles, npts ;
    QVector <double>xVec ;
    QVector <double>yVec ;
    void setAxisRange (int ax, float min, float max) ;
    void setXYData (int graph, float *xdata, float *ydata, int npts) ;
    void setXYData (int graph, double *xdata, double *ydata, int npts) ;
signals:

public slots:

};

#endif // MYPLOT_H
