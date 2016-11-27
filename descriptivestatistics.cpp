#include "descriptivestatistics.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


DescriptiveStatistics::DescriptiveStatistics()
{
    m_windowSize = INFINITE_WINDOW;
    m_currentSpot = 0;
}

DescriptiveStatistics::DescriptiveStatistics(int window) {
    setWindowSize(window);
    m_currentSpot = 0;
}

DescriptiveStatistics::DescriptiveStatistics(QList<double> initialDoubleArray) {
    if (initialDoubleArray.count() > 0) {
        foreach (const double d, initialDoubleArray) {
                m_DA.append(d);
        }
    }
}

void DescriptiveStatistics::addValue(double v) {
    if (m_windowSize != INFINITE_WINDOW) {
        if (getN() == m_windowSize) {
            addElementRolling(v);
        } else if (getN() < m_windowSize) {
            m_DA.append(v);
        }
    } else {
        m_DA.append(v);
    }
}

void DescriptiveStatistics::addElementRolling(double v){
    // Replace at the current spot (the oldest value)
    m_DA[m_currentSpot] = v;

    // Move the pointer
    if (++m_currentSpot == m_windowSize){
        // reset
        m_currentSpot = 0;
    }
}

/**
* Returns the <a href="http://www.xycoon.com/arithmetic_mean.htm">
* arithmetic mean </a> of the available values
* @return The mean or 0 if no values have been added.
*/
double DescriptiveStatistics::getMean() {
    return apply(&m_meanImpl);
}

/**
 * Returns the <a href="http://www.xycoon.com/geometric_mean.htm">
 * geometric mean </a> of the available values.
 * <p>
 * See {@link GeometricMean} for details on the computing algorithm.</p>
 *
 * @return The geometricMean, 0 if no values have been added,
 * or if any negative values have been added.
 */
double DescriptiveStatistics::getGeometricMean() {
    return apply(&m_geometricMeanImpl);
}

/**
* Returns the quadratic mean, a.k.a.
* <a href="http://mathworld.wolfram.com/Root-Mean-Square.html">
* root-mean-square</a> of the available values
* @return The quadratic mean or {@code Double.NaN} if no values
* have been added.
*/
double DescriptiveStatistics::getQuadraticMean() {
    long n = getN();
    return n > 0 ? std::sqrt(getSumsq() / n) : 0;
  }

/**
* Returns the maximum of the available values
* @return The max or Double.NaN if no values have been added.
*/
double DescriptiveStatistics::getMax() {
    return apply(&m_maxImpl);
}

/**
* Returns the minimum of the available values
* @return The min or Double.NaN if no values have been added.
*/
double DescriptiveStatistics::getMin() {
    return apply(&m_minImpl);
}

/**
* Returns the number of available values
* @return The number of available values
*/
long DescriptiveStatistics::getN() {
     return m_DA.count();
}

/**
* Returns the sum of the values that have been added to Univariate.
* @return The sum or Double.NaN if no values have been added
*/
double DescriptiveStatistics::getSum() {
    return apply(&m_sumImpl);
}

/**
* Returns the sum of the squares of the available values.
* @return The sum of the squares or Double.NaN if no
* values have been added.
*/
double DescriptiveStatistics::getSumsq() {
    return apply(&m_sumsqImpl);
}

void DescriptiveStatistics::clear() {
    m_DA.clear();
}

int DescriptiveStatistics::getWindowSize() {
    return m_windowSize;
}

void DescriptiveStatistics::setWindowSize(int windowSize){

    if (windowSize > 0 || windowSize == INFINITE_WINDOW) {

        // Set the size
        m_windowSize = windowSize;

        // We need to check to see if we need to discard elements
        // from the back of the array.  If the windowSize is less than
        // the current number of elements.
        if (m_windowSize != INFINITE_WINDOW && m_windowSize < m_DA.count()) {
            // start at the end
            int last_element = m_DA.count() - 1;
            for (int i = last_element; i >= m_windowSize; i--)
                m_DA.removeAt(i);
        }
    }
}

double DescriptiveStatistics::getElement(int index) {
    return m_DA.at(index);
}

double DescriptiveStatistics::DescriptiveStatistics::apply(IUnivariateStatistic* stat) {
    int len = m_windowSize;
    if (len == INFINITE_WINDOW)
        len = m_DA.count();
    return stat->evaluate(m_DA, 0, len);
}
