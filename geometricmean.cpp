#include "geometricmean.h"
#include "sumoflogs.h"

double GeometricMean::evaluate(QList<double> values){
    return evaluate(values, 0, values.count());
}

double GeometricMean::evaluate(QList<double> values, int begin, int length){
    return std::exp(
                SumOfLogs::evaluate(values, begin, length) / length);
}
