#include "parsedfstat.h"

Parsedfstat::Parsedfstat(QList<QFileInfo> name)
{
    statisticsfiles = name;
    list_header << "Filesystem" << "kbytes" << "used" \
            << "avail" << "capacity" << "Mounted" << "on";
}

Parsedfstat::Parsedfstat()
{
    list_header << "Filesystem" << "kbytes" << "used" \
            << "avail" << "capacity" << "Mounted" << "on";
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
                    double used = crtlist.at(list_header.indexOf("used")).toDouble(&ok1);
                    double total = crtlist.at(list_header.indexOf("kbytes")).toDouble(&ok2);
                    double avail = crtlist.at(list_header.indexOf("avail")).toDouble(&ok3);
                    QByteArray tmp = crtlist.at(list_header.indexOf("capacity"));
                    double capacity = tmp.remove(tmp.length()-1,1).toDouble(&ok4);
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

void Parsedfstat::setDatasourceInfo()
{
    for (int i = 0; i < ds.values.begin().value().size(); ++i) {
        ds.datasourcesMax << QString::number(100);
        ds.datasourcesMin << QString::number(0);
        ds.datasourcesName << QString::number(qChecksum(header.at(i), header.at(i).size()));
        ds.datasourcesType << stringDSType[yGAUGE];
     }
}
