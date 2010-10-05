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
    QString *list_devices;
    QList<unsigned int> values_positions, headers_positions;
    QMap< QString, double> mapsubblockvals;
    QMap<QString, QMap< QString, double> > mapblockvals;
    QStringList subheader;
    QRegExp regexp;
    quint64 max_nr_of_subblock_lines;
    int process_line();
    bool newBlock();
    void insertSubHeaders();
    void setTime();
    void setDatasourceInfo();
    bool matchLine(unsigned int);
    void insertValues();
    void insertLastValues();
    virtual void initdata() = 0;
};

#endif // PARSEPRTPICL_H
