#ifndef GEOMETRICMEAN_H
#define GEOMETRICMEAN_H

#include "UnivariateStatistic.h"
#include "qlist.h"

class GeometricMean : public IUnivariateStatistic
{
public:
    double evaluate(QList<double> values);
    double evaluate(QList<double> values, int begin, int length);
};

#endif // GEOMETRICMEAN_H
