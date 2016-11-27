#ifndef UNIVARIATESTATISTIC_H
#define UNIVARIATESTATISTIC_H

#include <QList>

class IUnivariateStatistic
{

public:
    virtual ~IUnivariateStatistic() {}
    virtual double evaluate(QList<double> values) = 0;
    virtual double evaluate(QList<double> values, int begin, int length) = 0;
};

#endif // UNIVARIATESTATISTIC_H
