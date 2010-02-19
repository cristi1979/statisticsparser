#include <QtCore/QCoreApplication>
#include "parseiostat.h"
#include "parsedfstat.h"
#include "parsesarstat.h"

bool first = true;

void myMessageOutput(QtMsgType type, const char* msg)
{
    QFile file("aaa_out");
    file.open(QIODevice::Append | QIODevice::Text);

    if ( first ) {
        first = false;
        file.remove();
    }

    QTextStream out(&file);
    switch (type) {
    case QtDebugMsg:
        if ( !QString::QString(msg).startsWith("kfile") ){
            fprintf(stderr, "Debug: %s\n", msg);
            out << "Debug: " << msg << endl;
        }
        break;
    case QtWarningMsg:
        fprintf(stderr, "Message: %s\n", msg);
        out << "Message: " << msg << endl;
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Error: %s\n", msg);
        out << "Error: " << msg << endl;
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        out << "Fatal: " << msg << endl;
        exit(3);
    }
    file.flush();
    file.close();
 }

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMsgHandler(myMessageOutput);

    QFileInfo file, rrd;
//    file.setFile("/home/cristi/work/parser/logs/aus1-mind_sarstat_1.log");
    file.setFile("d:\\temp\\parsers\\aus1-mind_sarstat_1.log");
//    file.setFile("d:\\temp\\parsers\\aus1-mind_dfstat_1.log");
//    file.setFile("d:\\temp\\parsers\\aus1-mind_iostat_1.log");
    rrd.setFile("coco.rrd");
//    Parseiostat q;
//    Parsedfstat q;
    Parsesarstat q;
    q.setStatsFilename(file);
    q.setRRDFileName(rrd);
    q.run();
    qDebug() << "FIN.";
    exit (10);
    return a.exec();
}
