#ifndef PARSEIOSTAT_H
#define PARSEIOSTAT_H

#include "parse.h"

class Parseiostat : public Parse
{
public:
    Parseiostat(QFileInfo name);
    Parseiostat();
    void setStatsFilename(QFileInfo name);
    int run();

private:
    QList <QByteArray> list_cpu, list_disk, devices;
    int process_line();
    void setTime();
};

#endif // PARSEIOSTAT_H
