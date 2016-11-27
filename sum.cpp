#include "sum.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

double Sum::evaluate(QList<double> values){
    return evaluate(values, 0, values.count());
}

double Sum::evaluate(QList<double> values, int begin, int length){
    double sum = 0.0;
    for (int i = begin; i < begin + length; i++) {
        sum += values[i];
    }
    return sum;
}
