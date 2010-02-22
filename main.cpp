#include <QtCore/QCoreApplication>
#include <getopt.h>
#include "parseiostat.h"
#include "parsedfstat.h"
#include "parsesarstat.h"
#include "parsefanstat.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "abc:")) != -1)
        switch (c)
        {
        case 'a':
        aflag = 1;
        break;
        case 'b':
        bflag = 1;
        break;
        case 'c':
        cvalue = optarg;
        break;
        case '?':
        if (optopt == 'c')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
            fprintf (stderr,
                     "Unknown option character `\\x%x'.\n",
                     optopt);
        return 1;
        default:
        abort ();
    }

    printf ("aflag = %d, bflag = %d, cvalue = %s\n",
            aflag, bflag, cvalue);

    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    return 0;

    QCoreApplication a(argc, argv);
    qInstallMsgHandler(myMessageOutput);
    for (int i=1;i<argc;i++){
        qDebug() << argc << argv[i];
    }
    exit(2);

    QFileInfo file, rrd;
#if defined (Q_OS_WIN32)
//    file.setFile("d:\\temp\\parsers\\aus1-mind_sarstat_1.log");
//    file.setFile("d:\\temp\\parsers\\aus1-mind_dfstat_1.log");
//    file.setFile("d:\\temp\\parsers\\aus1-mind_iostat_1.log");
    file.setFile("d:\\temp\\parsers\\aus1-mind_fanstat_1.log");
#else
    file.setFile("/home/cristi/work/parser/logs/aus1-mind_sarstat_1.log");
    file.setFile("/home/cristi/work/parser/logs/aus1-mind_fanstat_1.log");
//    file.setFile("/home/cristi/work/parser/logs/aus1-mind_dfstat_1.log");
//    file.setFile("/home/cristi/work/parser/logs/aus1-mind_iostat_1.log");
#endif
    rrd.setFile("coco.rrd");
//    Parseiostat q;
//    Parsedfstat q;
//    Parsesarstat q;
    Parsefanstat q;
    q.setStatsFilename(file);
    q.setRRDFileName(rrd);
    q.run();
    qDebug() << "FIN.";
    exit (10);
    return a.exec();
}
