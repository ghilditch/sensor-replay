#ifndef DESCRIPTIVESTATISTICS_H
#define DESCRIPTIVESTATISTICS_H

#include <QList>
#include "UnivariateStatistic.h"
#include "mean.h"
#include "geometricmean.h"
#include "max.h"
#include "min.h"
#include "sumofsquares.h"
#include "sum.h"

#define INFINITE_WINDOW -1

// Note: Based on code from http://commons.apache.org/proper/commons-math

class DescriptiveStatistics
{
public:
    DescriptiveStatistics();
    DescriptiveStatistics(int window);
    DescriptiveStatistics(QList<double> initialDoubleArray);

    void addValue(double v);
    double getMean();
    double getGeometricMean();
    double getQuadraticMean();
    double getMax();
    double getMin();
    long getN();
    double getSum();
    double getSumsq();
    void clear();
    int getWindowSize();
    void setWindowSize(int windowSize);
    double getElement(int index);
    double apply(IUnivariateStatistic* stat);

private:
    void addElementRolling(double);

private:
    // hold the window size
    int m_windowSize;

    // Current index
    int m_currentSpot;

    // Stored data values
    QList<double> m_DA;

    // Mean statistic implementation - can be reset by setter.
    Mean m_meanImpl;
    // Geometric mean statistic implementation - can be reset by setter.
    GeometricMean m_geometricMeanImpl;
    // Maximum statistic implementation - can be reset by setter.
    Max m_maxImpl;
    // Minimum statistic implementation - can be reset by setter.
    Min m_minImpl;
    // Sum of squares statistic implementation - can be reset by setter. */
    SumOfSquares m_sumsqImpl;
    // Sum statistic implementation - can be reset by setter.
    Sum m_sumImpl;
};

#endif // DESCRIPTIVESTATISTICS_H
