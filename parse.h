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

//http://docs.sun.com/app/docs/doc/819-2582/addgw?l=en&q=mpstat&a=view
const QString stringDSType[] = {
    "GAUGE",
    "COUNTER",
    "DERIVE",
    "ABSOLUTE"
};

class Parse
{
public:
    Parse(QList<QFileInfo> name);
    Parse();
    ~Parse();
    int run();
    void setStatsFilename(QList<QFileInfo> name);
    void setRRDFileName(QFileInfo name);

protected:
    quint64 lineNumber, blockNumber, intTime, error, blockLineNumber, lasttimestamp;
    QList<QFileInfo> statisticsfiles;
    QString block, line;
    QList<QString> header;
    QList<double> crtBlockValues;
    QList<double> getListDoubles(QStringList, bool set = true);

    struct datasource {
        QStringList datasourcesType, datasourcesName;
        QStringList datasourcesMin, datasourcesMax;
        QMap<quint64, QList<double> > values;
    } ds;
    enum typeDSType {
        yGAUGE,
        yCOUNTER,
        yDERIVE,
        yABSOLUTE,
        yDSTYPELAST
    } dstype;

    void setError(int, QString = "");
    void timeIncrement();
    void timeFromLine();
    void printMap();
    virtual void setDatasourceInfo();
    virtual void setTime();
    virtual int process_line() = 0;
    virtual bool newBlock();
    virtual void insertLastValues();
private:
    RRDTool *rrd;
    QRegExp date_regex;
    void sendToRRD();
    void clear();
    void init();
    bool isValidData();
};

#endif // PARSE_H
