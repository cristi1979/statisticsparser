#include <QtCore/QCoreApplication>
#include "parseiostat.h"
#include "parsedfstat.h"
#include "parsesarstat.h"
#include "parsefanstat.h"
#include "parsempstat.h"

#include <getopt.h>

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

void print_usage (QString program_name)
{
    qDebug() << "Usage:" << program_name << "options [ inputfiles ... ]";
    qDebug() <<
            "  -h  --help             Display this usage information.\n"
            "  -o  --output filename  Write output to file.\n"
            "  -v  --verbose          Print verbose messages.\n";
    exit (1);
}


void parse_arguments(int argc, char *argv[])
{
    int c;
    QString program_name = argv[0];

    while (1)
    {
        static struct option long_options[] =
        {
            {"coco",     required_argument,       0, 1},
            {"rada",  required_argument,       0, 2},
            {"dudu",  required_argument, 0, 3},
            {"output",  required_argument, 0, 4},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 1:
            printf ("option --coco with value `%s'\n", optarg);
            break;

        case 2:
            printf ("option -rada with value `%s'\n", optarg);
            break;

        case 3:
            printf ("option -dudu with value `%s'\n", optarg);
            break;

        case 4:
            printf ("option -output with value `%s'\n", optarg);
            break;

        case '?':
            /* getopt_long already printed an error message. */
            print_usage(program_name);
            break;

        default:
            abort ();
        }
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("files path: ");
        while (optind < argc)
            printf ("%s \n", argv[optind++]);
        putchar ('\n');
    }
}

int main(int argc, char *argv[])
{
    parse_arguments(argc, argv);

    QCoreApplication app(argc, argv);
    qInstallMsgHandler(myMessageOutput);

    QFileInfo file, rrd;
#if defined (Q_OS_WIN32)
    file.setFile("d:\\temp\\parsers\\aus1-mind_sarstat_1.log");
    //    file.setFile("d:\\temp\\parsers\\aus1-mind_dfstat_1.log");
    //    file.setFile("d:\\temp\\parsers\\aus1-mind_iostat_1.log");
    file.setFile("d:\\temp\\parsers\\aus1-mind_fanstat_1.log");
    file.setFile("d:\\temp\\parsers\\aus1-mind_mpstat_1.log");
#else
    file.setFile("/home/cristi/work/parser/logs/aus1-mind_sarstat_1.log");
    file.setFile("/home/cristi/work/parser/logs/aus1-mind_fanstat_1.log");
    file.setFile("/home/cristi/work/parser/logs/aus1-mind_mpstat_1.log");
//    file.setFile("/home/cristi/work/parser/logs/aus1-mind_dfstat_1.log");
    //    file.setFile("/home/cristi/work/parser/logs/aus1-mind_iostat_1.log");
#endif
    QList<QFileInfo> fileslist;
    fileslist << file;
    rrd.setFile("coco.rrd");
    //    Parseiostat q;
//    Parsedfstat q;
//    Parsesarstat q;
//    Parsefanstat q;
    Parsempstat q;
    q.setStatsFilename(fileslist);
    q.setRRDFileName(rrd);
    q.run();
    qDebug() << "FIN.";
    exit (10);
    return app.exec();
}
