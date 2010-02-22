#ifndef PARSESARSTAT_H
#define PARSESARSTAT_H

#include "parse.h"

class Parsesarstat : public Parse
{
public:
    Parsesarstat(QFileInfo name);
    Parsesarstat();
    void setStatsFilename(QFileInfo name);

private:
    QList<QByteArray> *list_devices;
    QDateTime firsttime;
    bool foundsecondheader, foundaheader;
    int headernumber;
    QTime crttime, prvtime;
    int process_line();
    void initdata();
    bool newBlock();
    void setTime();
    void getSZ(QList<QByteArray>);
    int getValues(QList<QByteArray> list);
    int getHeaders(QList<QByteArray> list);
    void buildHeaders();
    QList<double> getListDoubles(QList<QByteArray>, bool set = true);
};

#endif // PARSESARSTAT_H
