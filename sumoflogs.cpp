#include "sumoflogs.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

double SumOfLogs::evaluate(QList<double> values, int begin, int length){
    double result = 0.0;
    for (int i = begin; i < begin + length; i++) {
        result += std::log(values[i]);
    }
    return result;
}
