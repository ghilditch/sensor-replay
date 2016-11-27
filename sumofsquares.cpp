#include "sumofsquares.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


double SumOfSquares::evaluate(QList<double> values){
    return evaluate(values, 0, values.count());
}

double SumOfSquares::evaluate(QList<double> values, int begin, int length){
    double sumSq = 0.0;
    for (int i = begin; i < begin + length; i++) {
        sumSq += values[i] * values[i];
    }
    return sumSq;
}
