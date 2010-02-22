#include "parseprtpicl.h"

#define NUMBER_OF_SUBBLOCK_LINES 1

Parseprtpicl::Parseprtpicl()
{
}

void Parseprtpicl::setDatasourceInfo()
{
}

int Parseprtpicl::process_line()
{
    error = 0;
    if ( line.isEmpty() ) {
        blockLineNumber--;
        return 0;
    }
    unsigned int field;
    for (int i = 0; i < fields_of_values.size(); ++i) {
        field = fields_of_values.at(i) - 1;
        regexp.setPattern(list_devices[field]);
        if ( regexp.indexIn(line) >= 0 ) {
            qDebug()<<"=====" << regexp.cap(1)<<regexp.cap(2);
            break;
        }
    }
    regexp.setPattern(list_devices[fields_of_header-1]);
    if ( regexp.indexIn(line) >= 0 ) {
        qDebug()<<"___" << regexp.cap(1)<<regexp.cap(2);
    }
}

void Parseprtpicl::setTime()
{
    timeFromLine();
}
