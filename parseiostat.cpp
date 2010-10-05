#include "parseiostat.h"
using namespace std;

Parseiostat::Parseiostat(QList<QFileInfo> name)
{
    statisticsfiles = name;
    list_cpu << "us" << "sy" << "wt" << "id";
    list_disk << "r/s" << "w/s" << "kr/s" << "kw/s" << "wait" \
            << "actv" << "wsvc_t" << "asvc_t" << "%w" << "%b" << "device";
}

Parseiostat::Parseiostat()
{
    list_cpu << "us" << "sy" << "wt" << "id";
    list_disk << "r/s" << "w/s" << "kr/s" << "kw/s" << "wait" \
            << "actv" << "wsvc_t" << "asvc_t" << "%w" << "%b" << "device";
}

void Parseiostat::setTime()
{
    timeFromLine();
}

int Parseiostat::process_line()
{
    error = 0;
    if ( line.isEmpty() ) {
        blockLineNumber--;
        return 0;
    }
    switch (blockLineNumber) {
        case 0:{//cpu header
                if (line == "cpu") {
                    devices = line.split(',');
                } else {
                    setError(1, "first cpu header error.");
                }
            }
        break;
        case 1:{//cpu header
                QList<QString> crtlist = line.split(',');
                if (list_cpu == crtlist) {
                    for ( int i=0; i < crtlist.size(); ++i ){
                        header << devices[0] + "_" + crtlist.at(i);
                    }
                } else {
                    setError(1, "second cpu header error.");
                }
            }
        break;
        case 2:{//cpu values
                crtBlockValues << getListDoubles(line.simplified().split(' '));
            }
        break;
        case 3: {//extra header
                if (line != "extended device statistics") {
                    setError(1, "extended device statistics missing.");
                }
            }
        break;
        case 4:{//devices header
                devices = line.split(',');
                if ( list_disk != devices ){
                    setError(1);
                }
            }
        break;
        default:{//devices values
                QStringList crtlist = line.split(',');
                int size = crtlist.size();
                if ( devices.size() == size ) {
                    crtBlockValues << getListDoubles(crtlist);
                } else {
                    setError(1, "size of values different then size of devices.");
                    qCritical() << devices << crtlist;
                }
            }
        break;
    }
    return error;
}
