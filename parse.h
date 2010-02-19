#ifndef PARSE_H
#define PARSE_H

#include <QFile>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QRegExp>
#include <QtDebug>
#include <QMap>

#include "rrdtool.h"

#define SAMPLE_INTERVAL 30

const QString stringDSType[] = {
    "GAUGE",
    "COUNTER",
    "DERIVE",
    "ABSOLUTE"
};

class Parse
{
public:
    Parse(QByteArray name);
    Parse();
    int run();
    void setStatsFilename(QFileInfo name);
    void setRRDFileName(QFileInfo name);

protected:
    QFile statisticsfile;
    QByteArray block, line;
    QRegExp date_regex;

    struct datasource {
        QStringList datasourcesType, datasourcesName;
        QStringList datasourcesMin, datasourcesMax;
        QMap<quint64, QList<double> > values;
    } ds;
    QList<QByteArray> header;

    QList<double> crtBlockValues;
    quint64 lineNumber, blockNumber, intTime, error, blockLineNumber,
        sampleinterval, lasttimestamp;

    enum typeDSType {
        yGAUGE,
        yCOUNTER,
        yDERIVE,
        yABSOLUTE,
        yDSTYPELAST
    } dstype;

    RRDTool *rrd;

    void setError(int, QByteArray = "");
    void clear();
    void timeIncrement();
    void timeFromLine();
    void printMap();
    void sendToRRD();
    virtual void init();
    virtual void setTime();
    virtual int process_line() = 0;
    virtual void setDatasourceInfo();
    virtual bool newBlock();
};

#endif // PARSE_H
