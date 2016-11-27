#ifndef SUM_H
#define SUM_H

#include "UnivariateStatistic.h"
#include "qlist.h"

class Sum : public IUnivariateStatistic
{
public:
    double evaluate(QList<double> values);
    double evaluate(QList<double> values, int begin, int length);
};

#endif // SUM_H
