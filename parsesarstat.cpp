#include "parsesarstat.h"

#define NUMBER_OF_HEADER_LINES 14
#define SECONDS_IN_HOUR 3600
#define LINE_NR_DEVICE 1
#define LINE_NR_SZ 8
#define LINE_NR_MEM 13
#define DISK_PREFIX "disk_"

Parsesarstat::Parsesarstat(QList<QFileInfo> name)
{
    statisticsfiles = name;
    initdata();
}

Parsesarstat::Parsesarstat()
{
    initdata();
}

void Parsesarstat::initdata()
{
    list_devices = new QStringList[NUMBER_OF_HEADER_LINES];
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
    foundsecondheader = false;
    foundaheader = false;
    headernumber = 0;
}

/*
  New block for sar:
  - unix time stamp if blocknumber = 0
  - hh:mm:ss _something_
*/
bool Parsesarstat::newBlock()
{
    if ( blockNumber == 0 ) {
        return Parse::newBlock();
    } else {
        QList<QString> strtime = line.simplified().split(' ').at(0).split(':');
        if ( strtime.size() != 3 ) {
            return false;
        }
        if ( foundaheader)  {
            headernumber++;
            foundaheader = false;
        }
        QList<double> nrs = getListDoubles(strtime);
        crttime.setHMS(nrs.at(0), nrs.at(1), nrs.at(2));
        return !error && crttime.isValid(nrs.at(0), nrs.at(1), nrs.at(2));
    }
}

/*
  if blocknumber is 0 we have the timestamp from line
  else if intTime is defined
            if we are before the header it should be ~ the same
                time as above (+- SAMPLE_INTERVAL)
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
            if ( (dif % SAMPLE_INTERVAL) > SAMPLE_INTERVAL ) {
                qWarning() << "Difference between times is" << dif <<
                        "at line number" << lineNumber;
            }
            intTime += dif;
            prvtime = crttime;
            //lame. to keep the same block number
            blockNumber++;
            if (intTime > lasttimestamp) {
                process_line();
            }
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
    if ( foundsecondheader ) {
        setError(2, "We already found another header.");
        return error;
    }
    if ( blockNumber <= 1 ){
        return 0;
    }

    QList<QString> crtlist = line.simplified().split(' ');
    //if we are at the first line after the first block
    if ( (blockLineNumber == 0) && (blockNumber > 1) ) {
        if (crtlist.at(0).split(':').size() == 3) {
            crtlist.removeAt(0);
        }else {
            setError(1, "Every new block should start with the time.");
            return error;
        }
    }

    ( crtlist == list_devices[0] || foundaheader ) ?
            error = getHeaders(crtlist) :
                    error = getValues(crtlist);
    return error;
}

int Parsesarstat::getValues(QStringList list)
{
    error = 0;
    bool ok;
    QList<double> nrs;
    if ( !headernumber && (blockNumber > 1) ){
        setError(1, "We got values without knowing the header.");
        return error;
    }

    if ( blockLineNumber == 0) {
        if ( list.size() == list_devices[0].size() ){
            crtBlockValues << getListDoubles(list);
        } else {
            setError(1, "Wrong values.");
        }
    } else {
        list.at(0).toDouble(&ok);
        if ( !ok && headernumber ) {
            //check lines that don't start with a number
            nrs = getListDoubles(list.at(0).simplified().split('/'), false);
            if ( error || ( nrs.size() != 2 ) ){
                //disks
                if ( list.size() == list_devices[1].size() ) {
                    header << DISK_PREFIX + list.at(0);
                    list.removeAt(0);
                    crtBlockValues << getListDoubles(list);
                } else {
                    setError(1, "Number of devices does not match the header:");
                    qDebug() << list_devices[1] << list;
                }
            }else {
                getSZ(list);
            }
        } else {
            if ( list.size() == list_devices[blockLineNumber-header.size()+1].size() ){
                crtBlockValues << getListDoubles(list);
            }else {
                setError(1, "Number of devices does not match the header:");
                qCritical() << list_devices[blockLineNumber - header.size() + 1] << list;
            }
        }
    }
    return error;
}

int Parsesarstat::getHeaders(QStringList list)
{
    foundaheader = true;
    if ( headernumber ) {
        foundsecondheader = true;
        setError(2, "We think we found a second header. Not good.");
        return error;
    }
    if ( (blockLineNumber <= NUMBER_OF_HEADER_LINES) &&
         list_devices[blockLineNumber] == list ){
        if ( blockLineNumber == NUMBER_OF_HEADER_LINES ){
            foundaheader = false;
        }
        return 0;
    } else {
        foundsecondheader = true;
        setError(2, "Problems with the header");
        return error;
    }
}

QList<double> Parsesarstat::getListDoubles(QStringList list, bool set)
{
    error = 0;
    QList<double> listtmp;
    bool ok;

    for (int i=0; i< list.size(); i++){
        double nr = list.at(i).toDouble(&ok);
        if ( !ok && set ) {
            setError(1, "One of the fields is not a number:" + list.at(i));
            listtmp.clear();
            break;
        }
        listtmp << nr;
    }
    return listtmp;
}

void Parsesarstat::getSZ(QStringList list)
{
    QList<double> nrs;
    bool ok;
    if ( list.size() == list_devices[blockLineNumber - header.size() + 1].size() ){
//        crtBlockValues << nrs;
        for (int i = 0; i < list.size(); i++) {
            if (list.at(i).contains('/')) {
                nrs = getListDoubles(list.at(i).split('/'));
                if (!error){
                    if ( nrs.at(0) && nrs.at(1)){
                        crtBlockValues << nrs.at(0)/nrs.at(1)*100;
                    } else {
                        crtBlockValues << 0;
                    }
                }
             } else {
                 double nr = list.at(i).toDouble(&ok);
                 if (ok){
                     crtBlockValues << nr;
                 }
             }
         }
    }else {
        setError(1, "Number of devices does not match the header:");
        qCritical() << list_devices[blockLineNumber - header.size() + 1] << list;
    }
}

void Parsesarstat::insertLastValues()
{
    QList<QString> prvheader = header;
    header.clear();
    //the big header
    for (int i=0;i<NUMBER_OF_HEADER_LINES;i++){
        switch (i){
        case LINE_NR_DEVICE:{
                //the devices from the file
                for (int j=0;j<prvheader.size();j++) {
                    //the little header for devices
                    for (int k=1;k<list_devices[i].size();k++){
                        header << list_devices[i][0]+"_"+prvheader[j]+"_"+list_devices[i][k];
                    }
                }
            }
            break;
        case LINE_NR_SZ:{
                for (int k=0; k < list_devices[i].size() - 1; k+=2){
                    header << list_devices[i][k] << list_devices[i][k]+"_"+list_devices[i][k+1];
                }
                header << list_devices[i][list_devices[i].size()-1];
            }
            break;
        case LINE_NR_MEM:{
                for (int k=0; k < list_devices[i].size() - 2; k+=3){
                    header << list_devices[i][k] << list_devices[i][k]+"_"+list_devices[i][k+1]
                             << list_devices[i][k]+"_"+list_devices[i][k+2];
                }
                header << list_devices[i][list_devices[i].size()-2]
                        << list_devices[i][list_devices[i].size()-2]+"_"+list_devices[i][list_devices[i].size()-1];
            }
            break;
        default:{
                header << list_devices[i];
            }
        }
    }
}
