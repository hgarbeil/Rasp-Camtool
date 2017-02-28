#include "toplevel.h"
#include <QDebug>

TopLevel::TopLevel()
{
    int status ;
    ava = new Avaspec() ;
    status = ava->init() ;
    qDebug () << "Init status is : " << status ;
}
