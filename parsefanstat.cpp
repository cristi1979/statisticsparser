#include "parsefanstat.h"

#define MAX_NUMBER_OF_SUBBLOCK_LINES 8
#define FIELD_VAL_WARNING 5
#define FIELD_VAL_SPEED 6
#define FIELD_HDR_CLASS 7
#define FIELD_HDR_NAME 8

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
    max_nr_of_subblock_lines = MAX_NUMBER_OF_SUBBLOCK_LINES;
    list_devices = new QByteArray[max_nr_of_subblock_lines];
    int i = 0;
    list_devices[i++] = "^(.+)\\s+\\(fan,\\s+(\\d.+)\\)$";
    list_devices[i++] = "^:(_fru_parent)\\s+\\((.+)\\)$";
    list_devices[i++] = "^:(Label)\\s+(.+)$";
    list_devices[i++] = "^:(SpeedUnit)\\s+(rpm|okay)$";
    list_devices[i++] = "^:(LowWarningThreshold)\\s+(.+)$";
    list_devices[i++] = "^:(Speed)\\s+(.+)$";
    list_devices[i++] = "^:(_class)\\s.+(fan)$";
    list_devices[i++] = "^:(name)\\s+(.+)";
    values_positions << FIELD_VAL_WARNING << FIELD_VAL_SPEED;
    headers_positions << FIELD_HDR_CLASS << FIELD_HDR_NAME;
}
