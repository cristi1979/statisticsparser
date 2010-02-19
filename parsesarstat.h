#ifndef PARSESARSTAT_H
#define PARSESARSTAT_H

#include "parse.h"

class Parsesarstat : public Parse
{
public:
    Parsesarstat(QByteArray name);
    Parsesarstat();

private:
    QList<QByteArray> *list_devices;
    bool inheader;
    QDateTime firsttime;
    QTime crttime, prvtime;
    int process_line();
    void init();
    bool newBlock();
    void setTime();
    int getValues(QList<QByteArray> list);
    int getHeaders(QList<QByteArray> list);
};

#endif // PARSESARSTAT_H
