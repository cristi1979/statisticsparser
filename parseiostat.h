#ifndef PARSEIOSTAT_H
#define PARSEIOSTAT_H

#include "parse.h"

//http://docs.sun.com/app/docs/doc/819-2582/addgt?l=en&q=mpstat&a=view
class Parseiostat : public Parse
{
public:
    Parseiostat(QList<QFileInfo> name);
    Parseiostat();

private:
    QStringList list_cpu, list_disk, devices;
    int process_line();
    void setTime();
};

#endif // PARSEIOSTAT_H
