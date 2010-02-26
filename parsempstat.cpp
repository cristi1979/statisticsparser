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
}

bool Parsempstat::newBlock()
{
}

void Parsempstat::setTime()
{
    if ( blockNumber == 0 ) {
        timeFromLine();
//        firsttime = firsttime.fromTime_t(intTime);
//        prvtime = firsttime.time();
//        qDebug() << "Got first time.";
    } else {
        timeIncrement();
    }
//        if ( intTime > 0 ) {
            // prvtime should be the smallest. Always.
            // Also we trust the time from the file it's not after some hours
            // even if prvtime=23:59:59 and crttime=00:00:00
            // skip hours because of timezones
//            int dif = prvtime.secsTo(crttime) % SECONDS_IN_HOUR;
//            if ( dif < 0 ) {
//                dif += SECONDS_IN_HOUR;
//            }
//            if ( (dif % SAMPLE_INTERVAL) > SAMPLE_INTERVAL ) {
//                qWarning() << "Difference between times is" << dif <<
//                        "at line number" << lineNumber;
//            }
//            intTime += dif;
//            prvtime = crttime;
//            //lame. to keep the same block number
//            blockNumber++;
//            if (intTime > lasttimestamp) {
//                process_line();
//            }
//            blockNumber--;
//        } else {
//            setError(2, "We could't get the first time.");
//        }
//    }
    blockLineNumber++;
}
