#ifndef SUMOFLOGS_H
#define SUMOFLOGS_H

#include "qlist.h"

class SumOfLogs
{
public:

    static double evaluate(QList<double> values, int begin, int length);
};

#endif // SUMOFLOGS_H
