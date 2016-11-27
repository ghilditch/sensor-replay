#include "mean.h"
#include "sum.h"

double Mean::evaluate(QList<double> values){
    return evaluate(values, 0, values.count());
}

double Mean::evaluate(QList<double> values, int begin, int length){
    Sum sum;
    double sampleSize = length;

    // Compute initial estimate using definitional formula
    double xbar = sum.evaluate(values, begin, length) / sampleSize;

    // Compute correction factor in second pass
    double correction = 0;
    for (int i = begin; i < begin + length; i++) {
        correction += values[i] - xbar;
    }
    return xbar + (correction/sampleSize);
}
