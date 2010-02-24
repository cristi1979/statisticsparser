#ifndef PARSEPRTPICL_H
#define PARSEPRTPICL_H

#include "parse.h"

class Parseprtpicl : public Parse
{
public:
    Parseprtpicl(QList<QFileInfo> name);
    Parseprtpicl();
protected:
    QByteArray *list_devices;
    QList<unsigned int> values_positions, headers_positions;
    QMap<QByteArray, double>
    QList<QByteArray> subheader;
    QList<QByteArray> subheaderfromvalues;
    QRegExp regexp;
    quint64 nr_of_subblock_lines;
    int process_line();
    void setTime();
    void setDatasourceInfo();
    bool matchLine(unsigned int);
    virtual void initdata() = 0;
};

#endif // PARSEPRTPICL_H
