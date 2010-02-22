#include "parse.h"

Parse::Parse()
{
    init();
    qDebug() << "main empty";
}

Parse::Parse(QFileInfo name)
{
    init();
    statisticsfile.setFileName(name.absoluteFilePath());
    qDebug() << "main name";
}

Parse::~Parse()
{
    delete rrd;

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
    statisticsfile.close();

    if ( isValidData() ) {
        ds.values.insert(intTime, crtBlockValues);
    }
    buildHeaders();
    qWarning() << "Elapsed time in parser:" << runtime.elapsed();
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
        qCritical() << "\t" << header.size() << ds.values.begin().value().size() << header << ds.values.begin().value();
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
        ds.datasourcesName << QString::number(qChecksum(header.at(i), header.at(i).size()));
        qDebug() << header.at(i)<<QString::number(qChecksum(header.at(i), header.at(i).size()));
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
        qDebug() << rrd->getError() << rrd->getOutput();
        if ( rrd->getError().isEmpty() || (rrd->getError() == rrd->getExpectedError()) ) {
            rrd->update();
        }
    } else {
        setError(1, "Error sending data to rrd.");
    }
}

void Parse::buildHeaders()
{
}
