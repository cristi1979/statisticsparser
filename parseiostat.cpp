#include "parseiostat.h"
using namespace std;

Parseiostat::Parseiostat(QByteArray name)
{
    statisticsfile.setFileName(name);
    list_cpu << "us" << "sy" << "wt" << "id";
    list_disk << "r/s" << "w/s" << "kr/s" << "kw/s" << "wait" \
            << "actv" << "wsvc_t" << "asvc_t" << "%w" << "%b" << "device";
    qDebug() << "iostat name";
}

Parseiostat::Parseiostat()
{
    list_cpu << "us" << "sy" << "wt" << "id";
    list_disk << "r/s" << "w/s" << "kr/s" << "kw/s" << "wait" \
            << "actv" << "wsvc_t" << "asvc_t" << "%w" << "%b" << "device";
    qDebug() << "iostat empty";
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
        case 1:{//cpu header
                if (line == "cpu") {
                    devices = line.split(',');
                } else {
                    setError(1, "first cpu header error.");
                }
            }
        break;
        case 2:{//cpu header
                QList<QByteArray> crtlist = line.split(',');
                if (list_cpu == crtlist) {
                    for ( int i=0; i < crtlist.size(); ++i ){
                        header << devices[0] + "_" + crtlist.at(i);
                    }
                } else {
                    setError(1, "second cpu header error.");
                }
            }
        break;
        case 3:{//cpu values
                bool ok;
                foreach (QString str, line.split(',')){
                    crtBlockValues << str.toDouble(&ok);
                    if ( !ok ) {
                        setError(1, "cpu value not a number");
                    }
                }
            }
        break;
        case 4: {//extra header
                if (line != "extended device statistics") {
                    setError(1, "extended device statistics missing.");
                }
            }
        break;
        case 5:{//devices header
                devices = line.split(',');
                if ( list_disk != devices ){
                    setError(1);
                }
            }
        break;
        default:{//devices values
                QList<QByteArray> crtlist = line.split(',');
                int size = crtlist.size();
                if ( devices.size() == size ) {
                    bool ok;
                    for ( int i=0; i < size - 1; i++ ){
                        header << devices.at(i) + "_" + crtlist.at(size - 1);
                        crtBlockValues << crtlist.at(i).toDouble(&ok);
                        if ( !ok ) {
                            setError(1, "value not a number:" + crtlist.at(i));
                        }
                    }
                } else {
                    setError(1, "size of values different then size of devices.");
                }
            }
        break;
    }
    return error;
}

int Parseiostat::run()
{
    Parse::run();
//    printMap();
    return error;
}
