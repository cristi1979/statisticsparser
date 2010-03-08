#include "parsempstat.h"

Parsempstat::Parsempstat(QList<QFileInfo> name)
{
    statisticsfiles = name;
    list_header << "CPU" << "minf" << "mjf" << "xcal" << "intr" << "ithr" <<
            "csw" << "icsw" << "migr" << "smtx" << "srw" << "syscl" << "usr" << "sys" << "wt" << "idl";
}

Parsempstat::Parsempstat()
{
    list_header << "CPU" << "minf" << "mjf" << "xcal" << "intr" << "ithr" <<
            "csw" << "icsw" << "migr" << "smtx" << "srw" << "syscl" << "usr" << "sys" << "wt" << "idl";
}

int Parsempstat::process_line()
{
    QStringList crtlist = line.simplified().split(' ');
    if ( line.isEmpty() ){
        blockNumber--;
        return 0;
    }
    if ( crtlist.size() == list_header.size() ) {
        error = 0;
        if ( crtlist == list_header ){
            header = crtlist;
        } else {
            crtBlockValues = getListDoubles(crtlist);
        }
        return error;
    } else {
        setError(1, "Unknown line. All lines should have the same length.");
        return error;
    }
}

bool Parsempstat::newBlock()
{
    //every line is a new block
    process_line();
    return true;
}

void Parsempstat::setTime()
{
    if ( blockNumber == 0 ) {
        timeFromLine();
    } else {
        timeIncrement();
    }
    blockLineNumber++;
}
