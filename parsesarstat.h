#ifndef PARSESARSTAT_H
#define PARSESARSTAT_H

#include "parse.h"

class Parsesarstat : public Parse
{
public:
    Parsesarstat(QList<QFileInfo> name);
    Parsesarstat();

private:
    QStringList *list_devices;
    QDateTime firsttime;
    bool foundsecondheader, foundaheader;
    int headernumber;
    QTime crttime, prvtime;
    int process_line();
    void initdata();
    bool newBlock();
    void setTime();
    void getSZ(QStringList);
    int getValues(QStringList list);
    int getHeaders(QStringList list);
    void insertLastValues();
    QList<double> getListDoubles(QStringList, bool set = true);
};

#endif // PARSESARSTAT_H
