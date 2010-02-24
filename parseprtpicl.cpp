#include "parseprtpicl.h"

#define MAX_NUMBER_OF_SUBBLOCK_LINES 1

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

    if ( matchLine(0) ) {
        insertSubHeaders();
        return 0;
    }

    for (int i = 0; i < values_positions.size(); ++i) {
        if ( matchLine(values_positions.at(i) - 1) ) {
            bool ok;
            int val = regexp.cap(2).toInt(&ok, 16);
            if ( ok ) {
                mapsubblockvals.insert(regexp.cap(1).toAscii(), val);
                return 0;
            } else {
                setError(1, "value is not a number");
            }
        }
    }

    for (int i = 0; i < headers_positions.size(); ++i) {
        if ( matchLine(headers_positions.at(i) - 1) ) {
            subheader << regexp.cap(2).toAscii();
            return 0;
        }
    }

    for (int i=0; i<max_nr_of_subblock_lines; i++){
        if ( (values_positions.indexOf(i) < 0 || headers_positions.indexOf(i) < 0) && matchLine(i) ){
            return 0;
        }
    }
    setError(1, "unknown line.");
    return error;
}

void Parseprtpicl::insertSubHeaders()
{
    QByteArray tmphdr;
    //aka we are not at the begining of the block
    if (blockLineNumber) {
        for (int i=0;i<subheader.size(); i++){
            tmphdr += subheader.at(i);
        }
        mapblockvals.insert(tmphdr, mapsubblockvals);
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
        insertSubHeaders();
        QSet<QByteArray> uniquesetofvaluesheaders;
        QMapIterator<QByteArray, QMap< QByteArray, double> > i(mapblockvals);
        while (i.hasNext()) {
            i.next();
            QMapIterator<QByteArray, double> j(i.value());
            while (j.hasNext()) {
                j.next();
                uniquesetofvaluesheaders << j.key();
            }
        }
        i = mapblockvals;
        while (i.hasNext()) {
            i.next();
            QByteArray

//            QMapIterator<QByteArray, double> j(i.value());
//            while (j.hasNext()) {
//                j.next();
//                uniquesetofvaluesheaders << j.key();
//            }
        }
        qDebug() << "New block:" << mapblockvals;
        qDebug() << uniquesetofvaluesheaders;
         return true;
    } else {
        return false;
    }
}
