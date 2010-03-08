#include "parseprtpicl.h"

#define MAX_NUMBER_OF_SUBBLOCK_LINES 1

Parseprtpicl::Parseprtpicl()
{
}

void Parseprtpicl::setDatasourceInfo()
{
    int size = ds.values.begin().value().size();
    for (int i = 0; i < size; i++) {
        ds.datasourcesMax << QString::number(INVALID_VALUE_TO_DEFINE_UNKNOWN - 1);
        ds.datasourcesMin << "U";
        ds.datasourcesName << QString::number(qChecksum(header.at(i).toAscii(), header.at(i).size()));
        ds.datasourcesType << stringDSType[yGAUGE];
     }}

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

    if ( matchLine(0) ) {
        insertSubHeaders();
        return 0;
    }

    for (int i = 0; i < values_positions.size(); ++i) {
        if ( matchLine(values_positions.at(i) - 1) ) {
            bool ok;
            int val = regexp.cap(2).toInt(&ok, 16);
            if ( ok ) {
                mapsubblockvals.insert(regexp.cap(1), val);
                return 0;
            } else {
                setError(1, "value is not a number");
            }
        }
    }

    for (int i = 0; i < headers_positions.size(); ++i) {
        if ( matchLine(headers_positions.at(i) - 1) ) {
            subheader << regexp.cap(2).simplified();
            return 0;
        }
    }

    for (unsigned int i=0; i<max_nr_of_subblock_lines; i++){
        if ( (values_positions.indexOf(i) < 0 || headers_positions.indexOf(i) < 0) && matchLine(i) ){
            return 0;
        }
    }
    setError(1, "unknown line.");
    return error;
}

void Parseprtpicl::insertSubHeaders()
{
    //aka we are not at the begining of the block
    if (blockLineNumber) {
        mapblockvals.insert(subheader.join(":"), mapsubblockvals);
        mapsubblockvals.clear();
        subheader.clear();
    }
}

void Parseprtpicl::setTime()
{
    timeFromLine();
}


bool Parseprtpicl::newBlock()
{
    if ( Parse::newBlock() ) {
        insertValues();
         return true;
    } else {
        return false;
    }
}

void Parseprtpicl::insertValues()
{
    insertSubHeaders();
    QSet<QString> uniquesetofvaluesheaders;
    QMapIterator<QString, QMap< QString, double> > i(mapblockvals);
    while (i.hasNext()) {
        i.next();
        QMapIterator<QString, double> j(i.value());
        while (j.hasNext()) {
            j.next();
            uniquesetofvaluesheaders << j.key();
        }
    }
    i = mapblockvals;
    while (i.hasNext()) {
        i.next();
        QString tmp;
        foreach (tmp, uniquesetofvaluesheaders){
            header << i.key() + ":" + tmp;
            if ( i.value().contains(tmp) ){
                crtBlockValues << i.value().value(tmp);
            } else {
                crtBlockValues << INVALID_VALUE_TO_DEFINE_UNKNOWN;
            }
        }
    }
}

void Parseprtpicl::insertLastValues()
{
    insertValues();
}
