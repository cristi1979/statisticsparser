#include "parsefanstat.h"

Parsefanstat::Parsefanstat(QByteArray name)
{
    statisticsfile.setFileName(name);
}

Parsefanstat::Parsefanstat()
{
}
