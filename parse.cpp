#include "parse.h"

Parse::Parse()
{
    init();
}

Parse::Parse(QList<QFileInfo> name)
{
    init();
    statisticsfiles = name;
}

Parse::~Parse()
{
    delete rrd;

}

void Parse::setStatsFilename(QList<QFileInfo> name)
{
    statisticsfiles = name;
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
    dstype = yGAUGE;
    rrd = new RRDTool;
}

void Parse::setTime()
{
    timeFromLine();
}

void Parse::setError(int nr, QString string)
{
    error = nr;
    qCritical() << "Problem at line number" << QString::number(lineNumber) << ":" << string;
    qCritical() << line;
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
    intTime += SAMPLE_INTERVAL;
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
    QTime runtime;
    runtime.start();
    qWarning() << runtime.currentTime();

    QFile file;
    for (int i=0; i< statisticsfiles.size(); i++) {
        file.setFileName(statisticsfiles.at(i).absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical() << file.errorString() << " : "
                    << file.fileName() << "." << endl;
            return file.error();
        }
        clear();
        lasttimestamp = rrd->last().toInt();

        QList<QString> prev_header;
        while ( !file.atEnd() ) {
            line = file.readLine().trimmed();
            lineNumber++;
            if ( newBlock() ) {
                //new date: insert previous data, clear all
                if ( !prev_header.isEmpty() && (prev_header != header) ) {
                    setError(2, "Header error. We don't know which one is correct. Bail out.");
                    qCritical() << "previous header" << prev_header;
                    qCritical() << "current header" << header;
                    return error;
                }
                if ( isValidData() ) {
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
        file.close();
    }

    insertLastValues();
    if ( isValidData() ) {
        ds.values.insert(intTime, crtBlockValues);
    }
    qWarning() << "Elapsed time in parser:" << runtime.elapsed();
    qWarning() << "Number of blocks:" << blockNumber;
    runtime.restart();
    if ( ds.values.isEmpty()) {
        qDebug() << "Nothing to send to rrd";
        return error;
    }

    if ( header.size() == ds.values.begin().value().size() ) {
        sendToRRD();
        qWarning() << "Elapsed time in rrd:" << runtime.elapsed();
    } else {
        setError(1, "Headers not the same size as values.");
        qCritical() << header.size() << ds.values.begin().value().size() << header << ds.values.begin().value();
    }

    return error;
}

bool Parse::isValidData()
{
    return ( !crtBlockValues.isEmpty() && (intTime > 0) && !error &&
             //we don't need "or" below, because values.begin could be empty
             !( !ds.values.isEmpty()  &&
                (ds.values.begin().value().size() != crtBlockValues.size())
                ) );
}

void Parse::setDatasourceInfo()
{
    int size = ds.values.begin().value().size();
    for (int i = 0; i < size; i++) {
        ds.datasourcesMax << "U";
        ds.datasourcesMin << "U";
        ds.datasourcesName << QString::number(qChecksum(header.at(i).toAscii(), header.at(i).size()));
        ds.datasourcesType << stringDSType[yGAUGE];
     }
}

void Parse::sendToRRD()
{
    setDatasourceInfo();

    int size = ds.values.begin().value().size();
    if ( (ds.datasourcesMax.size() == size) ||
            (ds.datasourcesMin.size() == size) ||
            (ds.datasourcesName.size() == size) ||
            (ds.datasourcesType.size() == size) ) {
        rrd->setData(SAMPLE_INTERVAL,
                    ds.datasourcesType,
                    ds.datasourcesName,
                    ds.datasourcesMin,
                    ds.datasourcesMax,
                    ds.values);
        rrd->create();
        if ( rrd->getError().isEmpty() || (rrd->getError() == rrd->getExpectedError()) ) {
            rrd->update();
//            rrd->dump();
        } else {
            qWarning() << "rrd error:" << rrd->getError();
        }
    } else {
        setError(1, "Error sending data to rrd.");
    }
}

void Parse::insertLastValues()
{
}
