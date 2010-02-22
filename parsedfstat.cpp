#include "parsedfstat.h"

Parsedfstat::Parsedfstat(QFileInfo name)
{
    statisticsfile.setFileName(name.absoluteFilePath());
    list_header << "Filesystem" << "kbytes" << "used" \
            << "avail" << "capacity" << "Mounted" << "on";
}

Parsedfstat::Parsedfstat()
{
    list_header << "Filesystem" << "kbytes" << "used" \
            << "avail" << "capacity" << "Mounted" << "on";
}

void Parsedfstat::setStatsFilename(QFileInfo name)
{
    statisticsfile.setFileName(name.absoluteFilePath());
}

void Parsedfstat::setTime()
{
    timeFromLine();
}

int Parsedfstat::process_line()
{
    error = 0;
    if ( line.isEmpty() ) {
        blockLineNumber--;
        return 0;
    }
    QList<QByteArray> crtlist = line.simplified().split(' ');
    switch (blockLineNumber) {
        case 0:{//text
                if ( crtlist != list_header ) {
                    setError(2, "header is not correct.");
                }
            }
        break;
        default: {
                if ( crtlist.at(0).startsWith("/dev/") ) {
                    bool ok1, ok2, ok3, ok4;
                    quint64 used = crtlist.at(list_header.indexOf("used")).toULongLong(&ok1);
                    quint64 total = crtlist.at(list_header.indexOf("kbytes")).toULongLong(&ok2);
                    quint64 avail = crtlist.at(list_header.indexOf("avail")).toULongLong(&ok3);
                    QByteArray tmp = crtlist.at(list_header.indexOf("capacity"));
                    quint64 capacity = tmp.remove(tmp.length()-1,1).toULongLong(&ok4);
                    if ( ok1 && ok2 && ok3 && ok4 ) {
                        double val1 = (double)used * 100 / ((double)avail + (double)used) + 0.5;
                        double val2 = (double)used * 100 / (double)total + 0.5;
//                        qDebug() << "total" << QString::number(total) << \
//                                "used" << QString::number(used) << \
//                                "avail" << QString::number(avail) << \
//                                "capacity" << QString::number(capacity) << \
//                                "procent 1" << QString::number(val1,'g',4) << \
//                                "procent 2" << QString::number(val2,'g',4);
                        crtBlockValues << capacity;
                        header << crtlist.at(0) + " mounted at " + \
                                crtlist.at(crtlist.size()-1)\
                                + " with total size " + QString::number(total).toUtf8();
                    } else {
                        setError(1, "used or capacity are not numbers.");
                    }
                }
            }
        break;
    }

    return error;
}

int Parsedfstat::run()
{
    Parse::run();
//    printMap();
    return error;
}

void Parsedfstat::setDatasourceInfo()
{
    int size = ds.values.begin().value().size();
    for (int i = 0; i < size; ++i) {
        ds.datasourcesMax << QString::number(100);
        ds.datasourcesMin << QString::number(0);
        ds.datasourcesName << QString::number(qChecksum(header.at(i), header.at(i).size()));
        ds.datasourcesType << stringDSType[yGAUGE];
     }
}
