#ifndef MIN_H
#define MIN_H

#include "UnivariateStatistic.h"
#include "qlist.h"

class Min : public IUnivariateStatistic
{
public:
    double evaluate(QList<double> values);
    double evaluate(QList<double> values, int begin, int length);
};

#endif // MIN_H
