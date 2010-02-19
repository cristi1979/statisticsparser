#ifndef PARSEDFSTAT_H
#define PARSEDFSTAT_H

#include "parse.h"

class Parsedfstat : public Parse
{
public:
    Parsedfstat(QByteArray name);
    Parsedfstat();
    int run();

private:
    QList <QByteArray> list_header;
    int process_line();
    void setTime();
    void setDatasourceInfo();
};

#endif // PARSEDFSTAT_H
