#ifndef SUMOFSQUARES_H
#define SUMOFSQUARES_H

#include "UnivariateStatistic.h"
#include "qlist.h"

class SumOfSquares : public IUnivariateStatistic
{
public:
    double evaluate(QList<double> values);
    double evaluate(QList<double> values, int begin, int length);
};

#endif // SUMOFSQUARES_H
