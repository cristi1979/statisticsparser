#ifndef PARSEPRTPICL_H
#define PARSEPRTPICL_H

#include "parse.h"

class Parseprtpicl : public Parse
{
public:
    Parseprtpicl();
private:
    QList <QByteArray> list_coco;
    int process_line();
    void setTime();
    void setDatasourceInfo();
};

#endif // PARSEPRTPICL_H
