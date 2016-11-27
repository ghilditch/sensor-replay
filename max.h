#ifndef MAX_H
#define MAX_H

#include "UnivariateStatistic.h"
#include "qlist.h"

class Max : public IUnivariateStatistic
{
public:
    double evaluate(QList<double> values);
    double evaluate(QList<double> values, int begin, int length);
};

#endif // MAX_H
