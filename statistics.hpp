#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <QtGlobal>

class Statistics
{
public:
    Statistics();
    Statistics(const Statistics &x);

    Statistics &operator=(const Statistics &x);

    void append(int value);
    void update();
    void clear();

    int count() const;
    const int *histgram() const;
    int histgram(int i) const;
    int histgramMax() const;
    int histgramMin() const;
    qreal mean() const;
    qreal stddev() const;
    qreal median() const;
    qreal variance() const;
    qreal sum() const;
    int min() const;
    int max() const;
    int mode() const;

private:
    int _count;
    int _histgram[256];
    qreal _sum;
    qreal _variance;
    qreal _mean;
    qreal _median;
    int _min;
    int _max;
};

#endif // STATISTICS_HPP
