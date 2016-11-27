#include "max.h"


double Max::evaluate(QList<double> values){
    return evaluate(values, 0, values.count());
}

double Max::evaluate(QList<double> values, int begin, int length){
    double max = values[begin];
    for (int i = begin; i < begin + length; i++) {
        max = (max > values[i]) ? max : values[i];
    }
    return max;
}
