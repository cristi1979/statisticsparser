#ifndef PARSEFANSTAT_H
#define PARSEFANSTAT_H

#include "parseprtpicl.h"

class Parsefanstat : public Parseprtpicl
{
public:
    Parsefanstat(QByteArray);
    Parsefanstat();
    int run();

private:
};

#endif // PARSEFANSTAT_H
