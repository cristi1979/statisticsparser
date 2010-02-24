#include "parseprtpicl.h"

#define NUMBER_OF_SUBBLOCK_LINES 1

Parseprtpicl::Parseprtpicl()
{
}

void Parseprtpicl::setDatasourceInfo()
{
}

bool Parseprtpicl::matchLine(unsigned int pos)
{
    regexp.setPattern(list_devices[pos]);
    return regexp.indexIn(line) >= 0;
}

int Parseprtpicl::process_line()
{
    error = 0;
    if ( line.isEmpty() ) {
        blockLineNumber--;
        return 0;
    }
    unsigned int pos = 0;

    if ( matchLine(pos) ) {
        qDebug() << "New subblock:" << subheaderfromvalues << subheader<<crtBlockValues;
        subheader.clear();
//        subheaderfromvalues.clear();
        return 0;
    }

    for (int i = 0; i < values_positions.size(); ++i) {
        pos = values_positions.at(i) - 1;
        if ( matchLine(pos) ) {
            bool ok;
            crtBlockValues << regexp.cap(2).toInt(&ok, 16);
            if ( ok ) {
                QByteArray tmp = regexp.cap(1).toAscii();
                subheaderfromvalues.replace(i, tmp);
                return 0;
            } else {
                setError(1, "value is not a number");
            }
        }
    }

    for (int i = 0; i < headers_positions.size(); ++i) {
        pos = headers_positions.at(i) - 1;
        if ( matchLine(pos) ) {
            subheader << regexp.cap(2).toAscii();
            return 0;
        }
    }
    return 0;
}

void Parseprtpicl::setTime()
{
    timeFromLine();
}
