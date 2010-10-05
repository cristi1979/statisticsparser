#ifndef RRDTOOL_H
#define RRDTOOL_H

#include <QProcess>
#include <QDate>
#include <QMap>
#include <QDir>
#include <QtDebug>

class RRDTool
{
public:
    RRDTool();
    ~RRDTool();

    void create();
    void update();
    void fetch(quint64,quint64,quint64);
    void graph();
    void resize();
    void tune();
    void info();
    QString last();
    void first();
    void dump();
    void restore();

    int getReturnCode();
    void setData(const int interval,
                 const QStringList datasourcesType,
                 const QStringList datasourcesName,
                 const QStringList datasourcesMin,
                 const QStringList datasourcesMax,
                 const QMap<quint64, QList<double> > values);
    void setRRDFileName(const QFileInfo rrdfilename);
    QString getError();
    QString getOutput();
    QString getExpectedError();
    QString getExpectedOutput();

private:
    QFileInfo rrddatabasename;
    QString stringError, stringOutput, stringExpectedError, stringExpectedOutput, here;
    QProcess *process;
    QTime time;
    quint64 databasestep;

    enum typeTimePeriod {tMinute,
                       tHour,
                       tDay,
                       tWeek,
                       tMonth,
                       tYear };
    QMap <QString, typeTimePeriod> period;

    struct datasourceType {
        QStringList datasourcesType, datasourcesName;
        QStringList datasourcesMin, datasourcesMax;
        QMap<quint64, QList<double> > values;
    };
    datasourceType ds;


    int timeinseconds(int nr, QString time);
    QStringList createRRA();
    void run(QStringList stringArguments);
};

#endif // RRDTOOL_H
