#include "parsefanstat.h"

#define NUMBER_OF_SUBBLOCK_LINES 8
#define FIELD_WARNING 5
#define FIELD_SPEED 6
#define FIELD_CLASS 7
#define FIELD_NAME 8

Parsefanstat::Parsefanstat(QList<QFileInfo> name)
{
    statisticsfiles = name;
    initdata();
}

Parsefanstat::Parsefanstat()
{
    initdata();
}

void Parsefanstat::initdata()
{
    nr_of_subblock_lines = NUMBER_OF_SUBBLOCK_LINES;
    list_devices = new QByteArray[nr_of_subblock_lines];
    int i = 0;
    list_devices[i++] = "^(.+)\\s+\\(fan,\\s+(\\d.+)\\)$";
    list_devices[i++] = "^:(_fru_parent)\\s+\\((.+)\\)$";
    list_devices[i++] = "^:(Label)\\s+(.+)$";
    list_devices[i++] = "^:(SpeedUnit\\s+rpm)$";
    list_devices[i++] = "^:(LowWarningThreshold)\\s+(.+)$";
    list_devices[i++] = "^:(Speed)\\s+(.+)$";
    list_devices[i++] = "^:(_class)\\s.+(fan)$";
    list_devices[i++] = "^:(name)\\s+(.+)";
    values_positions << FIELD_WARNING << FIELD_SPEED;
    headers_positions << FIELD_NAME << FIELD_CLASS;
}

bool Parsefanstat::newBlock()
{
    if ( Parse::newBlock() ) {
        qDebug() << "New subblock:" << crtBlockValues << header;
        return true;
    } else {
        return false;
    }
}
