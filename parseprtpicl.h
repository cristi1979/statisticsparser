#ifndef PARSEPRTPICL_H
#define PARSEPRTPICL_H

#include "parse.h"

#define INVALID_VALUE_TO_DEFINE_UNKNOWN 1000000000

class Parseprtpicl : public Parse
{
public:
    Parseprtpicl(QList<QFileInfo> name);
    Parseprtpicl();
protected:
    QByteArray *list_devices;
    QList<unsigned int> values_positions, headers_positions;
    QMap< QByteArray, double> mapsubblockvals;
    QMap<QByteArray, QMap< QByteArray, double> > mapblockvals;
    QList<QByteArray> subheader;
    QRegExp regexp;
    quint64 max_nr_of_subblock_lines;
    int process_line();
    bool newBlock();
    void insertSubHeaders();
    void setTime();
    void setDatasourceInfo();
    bool matchLine(unsigned int);
    virtual void initdata() = 0;
};

#endif // PARSEPRTPICL_H
