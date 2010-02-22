#ifndef PARSEFANSTAT_H
#define PARSEFANSTAT_H

#include "parseprtpicl.h"
#include "parse.h"

class Parsefanstat : public Parseprtpicl
{
public:
    Parsefanstat(QFileInfo);
    Parsefanstat();
    void setStatsFilename(QFileInfo name);
    int run();
private:
    void initdata();
};

#endif // PARSEFANSTAT_H
