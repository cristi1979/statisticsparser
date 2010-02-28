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
    QList<QString> crtlist = line.simplified().split(' ');
    switch (blockLineNumber) {
        case 0:{//text
                if ( crtlist != list_header ) {
                    setError(2, "header is not correct.");
                }
            }
        break;
        default: {
                if ( crtlist.at(0).startsWith("/dev/") ) {
                    QString tmp = crtlist.at(list_header.indexOf("capacity"));
                    tmp.chop(1);
                    QStringList tmplist;
                    tmplist << crtlist.at(list_header.indexOf("used"))
                            << crtlist.at(list_header.indexOf("kbytes"))
                            << crtlist.at(list_header.indexOf("avail"))
                            << tmp;
                    QList<double> allvals = getListDoubles(tmplist);
                    if ( !allvals.isEmpty() ) {
                        crtBlockValues << allvals.last();
                        header << crtlist.at(0) + " mounted at " + \
                                crtlist.at(crtlist.size()-1)\
                                + " with total size " + crtlist.at(list_header.indexOf("kbytes"));
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
        ds.datasourcesName << QString::number(qChecksum(header.at(i).toAscii(), header.at(i).size()));
        ds.datasourcesType << stringDSType[yGAUGE];
     }
}
