#include "parse.h"

Parse::Parse()
{
    init();
    qDebug() << "main empty";
}

Parse::Parse(QByteArray name)
{
    init();
    statisticsfile.setFileName(name);
    qDebug() << "main name";
}

void Parse::setStatsFilename(QFileInfo name)
{
    statisticsfile.setFileName(name.absoluteFilePath());
}

void Parse::setRRDFileName(QFileInfo name)
{
    rrd->setRRDFileName(name);
}

void Parse::init()
{
    date_regex.setPattern("^\\d{10}\\s*$");
    lineNumber = 0;
    blockNumber = 0;
    intTime = 0;
    error = 0;
    sampleinterval = SAMPLE_INTERVAL;
    dstype = yGAUGE;
    rrd = new RRDTool;
}

void Parse::setTime()
{
    timeFromLine();
}

void Parse::setError(int nr, QByteArray string)
{
    error = nr;
    qCritical() << "Problem at line number" << QString::number(lineNumber) << ":" << string;
}

void Parse::clear()
{
    crtBlockValues.clear();
    header.clear();
    block.clear();
    blockLineNumber = 0;
    error = 0;
}

void Parse::timeFromLine()
{
    intTime = line.simplified().toInt();
}

void Parse::timeIncrement()
{
    intTime += sampleinterval;
}

void Parse::printMap()
{
    QMapIterator<quint64, QList<double> > i(ds.values);
    qWarning() << "time" << header;
    while (i.hasNext()) {
        i.next();
        qWarning() << i.key() << ":" << i.value();
     }
}

bool Parse::newBlock()
{
    return date_regex.indexIn(line) >= 0;
}

int Parse::run()
{
    qWarning() << QTime::currentTime();

    if (!statisticsfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << statisticsfile.errorString() << " : "
                << statisticsfile.fileName() << "." << endl;
        return statisticsfile.error();
    }
    clear();
    lasttimestamp = rrd->last().toInt();

    QList<QByteArray> prev_header;
    while ( !statisticsfile.atEnd() ) {
        line = statisticsfile.readLine().trimmed();
        lineNumber++;
        if ( newBlock() ) {
            //new date: insert previous data, clear all
            if ( !prev_header.isEmpty() && prev_header != header ) {
                setError(2, "Header error. We don't know which one is correct. Bail out.");
                qCritical() << "previous header" << prev_header;
                qCritical() << "current header" << header;
                return error;
            }
            if ( ( !crtBlockValues.isEmpty() && intTime > 0 && !error &&
                   //we don't need "or" below, because values.begin could be empty
                   !( !ds.values.isEmpty()  &&
                      (ds.values.begin().value().size() != crtBlockValues.size()) )
                ) ) {
                ds.values.insert(intTime, crtBlockValues);
            } else {
                if ( !crtBlockValues.isEmpty() ) {
                    setError(1, "Error inserting the data until this line." );
                }
            }

            prev_header = header;
            clear();

            setTime();
            blockNumber++;
            block += QString::number(intTime) + "\n";
        } else if ( !error ) {
            //in data block
            block += line + "\n";
            if ( (intTime > lasttimestamp) && !process_line() ) {
                blockLineNumber++;
            }
        }
    }

    if ( !crtBlockValues.isEmpty() && !error ) {
        ds.values.insert(intTime, crtBlockValues);
    }

    sendToRRD();
    statisticsfile.close();

    qWarning() << QTime::currentTime();
    return error;
}

void Parse::setDatasourceInfo()
{
    int size = ds.values.begin().value().size();
    for (int i = 0; i < size; ++i) {
        ds.datasourcesMax << "U";
        ds.datasourcesMin << "U";
        ds.datasourcesName << QString::number(qChecksum(header.at(i), header.at(i).size()));
        ds.datasourcesType << stringDSType[yGAUGE];
     }
}

void Parse::sendToRRD()
{
    if ( ds.values.isEmpty()) {
        qDebug() << "Nothing to send to rrd";
        return;
    }
    setDatasourceInfo();

    int size = ds.values.begin().value().size();
    if (ds.datasourcesMax.size() == size ||
            ds.datasourcesMin.size() == size ||
            ds.datasourcesName.size() == size ||
            ds.datasourcesType.size() == size) {
        rrd->setData(sampleinterval,
                    ds.datasourcesType,
                    ds.datasourcesName,
                    ds.datasourcesMin,
                    ds.datasourcesMax,
                    ds.values);
        rrd->create();
        qDebug() << rrd->getError() << rrd->getOutput();
        if ( rrd->getError().isEmpty() || (rrd->getError() == rrd->getExpectedError()) ) {
//            rrd->update();
        }
    } else {
        setError(1, "Error sending data to rrd.");
    }
}
