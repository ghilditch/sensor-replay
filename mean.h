#ifndef MEAN_H
#define MEAN_H

#include "UnivariateStatistic.h"
#include "qlist.h"

class Mean : public IUnivariateStatistic
{
public:
    double evaluate(QList<double> values);
    double evaluate(QList<double> values, int begin, int length);
};

#endif // MEAN_H
