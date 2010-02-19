#include "parseprtpicl.h"

Parseprtpicl::Parseprtpicl()
{
}

void Parseprtpicl::setDatasourceInfo()
{
}

int Parseprtpicl::process_line()
{
    error = 0;
    if ( line.isEmpty() ) {
        return 0;
    }
}

void Parseprtpicl::setTime()
{
    timeFromLine();
}
