#ifndef PARSEMPSTAT_H
#define PARSEMPSTAT_H

#include "parse.h"

//http://docs.sun.com/app/docs/doc/819-2582/addgp?l=en&a=view&q=mpstat
class Parsempstat : public Parse
{
public:
    Parsempstat(QList<QFileInfo> name);
    Parsempstat();
private:
    QStringList list_header;
    int process_line();
    bool newBlock();
    void setTime();
};

#endif // PARSEMPSTAT_H
