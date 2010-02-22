#ifndef PARSEPRTPICL_H
#define PARSEPRTPICL_H

#include "parse.h"

class Parseprtpicl : public Parse
{
public:
    Parseprtpicl(QFileInfo name);
    Parseprtpicl();
protected:
    QByteArray *list_devices;
    QList<unsigned int> fields_of_values;
    unsigned int fields_of_header;
    QRegExp regexp;
    quint64 nr_of_subblock_lines;
    int process_line();
    void setTime();
    void setDatasourceInfo();
    virtual void initdata() = 0;
};

#endif // PARSEPRTPICL_H
