#include "parsefanstat.h"

#define NUMBER_OF_SUBBLOCK_LINES 8
#define FIELD_WARNING 5
#define FIELD_SPEED 6
#define FIELD_NAME 8

Parsefanstat::Parsefanstat(QFileInfo name)
{
    statisticsfile.setFileName(name.absoluteFilePath());
    initdata();
}

Parsefanstat::Parsefanstat()
{
    initdata();
}

void Parsefanstat::setStatsFilename(QFileInfo name)
{
    statisticsfile.setFileName(name.absoluteFilePath());
}

void Parsefanstat::initdata()
{
    nr_of_subblock_lines = NUMBER_OF_SUBBLOCK_LINES;
    list_devices = new QByteArray[nr_of_subblock_lines];
    int i = 0;
    list_devices[i++] = "^(.+)\\s+\\(fan,\\s+(.+)\\)$";
    list_devices[i++] = "^:(_fru_parent)\\s+\\((.+)\\)$";
    list_devices[i++] = "^:(Label)\\s+(.+)$";
    list_devices[i++] = "^:(SpeedUnit\\s+rpm)$";
    list_devices[i++] = "^:(LowWarningThreshold)\\s+(.+)$";
    list_devices[i++] = "^:(Speed)\\s+(.+)$";
    list_devices[i++] = "^:(_class\\s.+fan)$";
    list_devices[i++] = "^:(name)\\s+(.+)";
    fields_of_values << FIELD_WARNING << FIELD_SPEED;
    fields_of_header = FIELD_NAME;
}

int Parsefanstat::run()
{
    Parse::run();
}
