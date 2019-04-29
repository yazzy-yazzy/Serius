#include "statistics.hpp"

#include <QtCore>

#include <type_traits>

template <typename T, int nSize>
T getMedian(T histgram[], T count)
{
    auto refCount = 0;
    auto medianIndex = count / 2;
    for (auto i = 0; i < nSize; i++) {
        refCount += histgram[i];
        if (refCount >= medianIndex)
            return i;
    }
    return 0;
}

Statistics::Statistics()
{
    clear();
}

void Statistics::clear()
{
    for (int i = 0; i < 256; i++) _histgram[i] = 0;
    _mean = _median = _sum = _variance = 0.0;
    _count = 0;

    _max = std::numeric_limits<int>::min();
    _min = std::numeric_limits<int>::max();
}

void Statistics::append(int value)
{
    value = qBound(0, value, 255);

    _histgram[value]++;
    _sum += value;
    _variance += value * value;
    _count++;
    _min = qMin(_min, value);
    _max = qMax(_max, value);
}

void Statistics::update()
{
    _median = getMedian<int, 256>(_histgram, _count);
    _mean = _sum / _count;
    _variance = (_variance / _count) - (_mean * _mean);
}

int Statistics::count() const
{
    return _count;
}

const int *Statistics::histgram() const
{
    return _histgram;
}

int Statistics::histgram(int i) const
{
    return _histgram[i];
}

qreal Statistics::mean() const
{
    return _mean;
}

qreal Statistics::stddev() const
{
    return qSqrt(_variance);
}

qreal Statistics::median() const
{
    return _median;
}

qreal Statistics::variance() const
{
    return _variance;
}

qreal Statistics::sum() const
{
    return _sum;
}

int Statistics::min() const
{
    return _min;
}

int Statistics::max() const
{
    return _max;
}

int Statistics::mode() const
{
    return std::distance(std::begin(_histgram), std::max_element(std::begin(_histgram), std::end(_histgram)));
}

int Statistics::histgramMax() const
{
    return *(std::max_element(std::begin(_histgram), std::end(_histgram)));
}

int Statistics::histgramMin() const
{
    return *(std::min_element(std::begin(_histgram), std::end(_histgram)));
}
