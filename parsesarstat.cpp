#include "parsesarstat.h"

#define NUMBER_OF_HEADER_LINES 14
#define SECONDS_IN_HOUR 3600

Parsesarstat::Parsesarstat(QByteArray name)
{
    statisticsfile.setFileName(name);
    init();
}

Parsesarstat::Parsesarstat()
{
    init();
}

void Parsesarstat::init()
{
    list_devices = new QList<QByteArray>[NUMBER_OF_HEADER_LINES];
    int i = 0;
    list_devices[i++] << "%usr" << "%sys" << "%wio" << "%idle";
    list_devices[i++] << "device" << "%busy" << "avque" << "r+w/s" << "blks/s" <<
            "avwait" << "avserv";
    list_devices[i++] << "runq-sz" << "%runocc" << "swpq-sz" << "%swpocc";
    list_devices[i++] << "bread/s" << "lread/s" << "%rcache" << "bwrit/s" <<
            "lwrit/s" << "%wcache" << "pread/s" << "pwrit/s";
    list_devices[i++] << "swpin/s" << "bswin/s" << "swpot/s" << "bswot/s" << "pswch/s";
    list_devices[i++] << "scall/s" << "sread/s" << "swrit/s" << "fork/s" << "exec/s" <<
            "rchar/s" << "wchar/s";
    list_devices[i++] << "iget/s" << "namei/s" << "dirbk/s";
    list_devices[i++] << "rawch/s" << "canch/s" << "outch/s" << "rcvin/s" << "xmtin/s" <<
            "mdmin/s";
    list_devices[i++] << "proc-sz" << "ov" << "inod-sz" << "ov" << "file-sz" << "ov" << "lock-sz";
    list_devices[i++] << "msg/s" << "sema/s";
    list_devices[i++] << "atch/s" << "pgin/s" << "ppgin/s" << "pflt/s" << "vflt/s" << "slock/s";
    list_devices[i++] << "pgout/s" << "ppgout/s" << "pgfree/s" << "pgscan/s" << "%ufs_ipf";
    list_devices[i++] << "freemem" << "freeswap";
    list_devices[i++] << "sml_mem" << "alloc" << "fail" << "lg_mem" << "alloc" << "fail" <<
            "ovsz_alloc" << "fail";
    inheader = false;
}

/*
  New block for sar:
  - unix time stamp if blocknumber = 0
  - hh:mm:ss _something_
*/
bool Parsesarstat::newBlock()
{
    if ( blockNumber == 0 ) {
        return date_regex.indexIn(line) >= 0;
    } else {
        bool ok1, ok2, ok3;
        QList<QByteArray> strtime = line.simplified().split(' ').at(0).split(':');
        if ( strtime.size() != 3 ) {
            return false;
        }
        int hour = strtime.at(0).toInt(&ok1),
            min = strtime.at(1).toInt(&ok2),
            sec = strtime.at(2).toInt(&ok3);
        crttime.setHMS(hour, min, sec);
        bool ok = ok1 && ok2 && ok3 && crttime.isValid(hour, min, sec);
        if ( ok && inheader ) {
            inheader = false;
        }
        return ok;
    }
}

/*
  if blocknumber is 0 we have the timestamp from line
  else if intTime is defined
            if we are before the header it should be ~ the same time as above (+- SAMPLE_INTERVAL)
            else we add the difference between the times
       else we are in misery
*/
void Parsesarstat::setTime()
{
    if ( blockNumber == 0 ) {
        timeFromLine();
        firsttime = firsttime.fromTime_t(intTime);
        prvtime = firsttime.time();
        qDebug() << "Got first time.";
    } else {
        if ( intTime > 0 ) {
            // prvtime should be the smallest. Always.
            // Also we trust the time from the file it's not after some hours
            // even if prvtime=23:59:59 and crttime=00:00:00
            // skip hours because of timezones
            int dif = prvtime.secsTo(crttime) % SECONDS_IN_HOUR;
            if ( dif < 0 ) {
                dif += SECONDS_IN_HOUR;
            }
            if ( dif % 30 ) {
                qDebug() << "Difference between times is" << dif << "at" << lineNumber;
            }
            intTime += dif;
            prvtime = crttime;
            //lame. to keep the same block number
            blockNumber++;
            process_line();
            blockNumber--;
        } else {
            setError(2, "We could't get the first time.");
        }
    }
    blockLineNumber++;
}

int Parsesarstat::process_line()
{
    error = 0;

    if ( line.isEmpty() ) {
        blockLineNumber--;
        return 0;
    }
    QList<QByteArray> crtlist = line.simplified().split(' ');
    if ( blockLineNumber == 0 && !error && blockNumber > 1 ) {
        crtlist.removeAt(0);
        if ( ! inheader && crtlist == list_devices[0] && header.isEmpty() ) {
            inheader = true;
        }
    }

    inheader ? error = getHeaders(crtlist) : error = getValues(crtlist);

    qDebug() << blockNumber << blockLineNumber << !error<< crtlist;
    return error;
}

int Parsesarstat::getValues(QList<QByteArray> list)
{
    QList<QByteArray> crtlist = line.simplified().split(' ');
    int size = crtlist.size();
    if ( list_devices[0].size() == size-1 ) {
        for ( int i=1; i < size - 1; i++ ){
//            crtBlockValues << crtlist.at(i).toDouble(&ok);
//            if ( !ok ) {
//                setError(1, "value not a number:" + crtlist.at(i));
//            }
        }
    } else {
        setError(1, "size of values different then size of devices.");
    }
}

int Parsesarstat::getHeaders(QList<QByteArray> list)
{
    return list_devices[blockLineNumber] == list;
}
