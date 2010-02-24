#ifndef PARSEFANSTAT_H
#define PARSEFANSTAT_H

#include "parseprtpicl.h"
#include "parse.h"

class Parsefanstat : public Parseprtpicl
{
public:
    Parsefanstat(QList<QFileInfo>);
    Parsefanstat();
private:
    void initdata();
    bool newBlock();
};

#endif // PARSEFANSTAT_H
