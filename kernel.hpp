#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <QGenericMatrix>

class Kernel : public QGenericMatrix<5, 5, int>
{
public:
    Kernel() : QGenericMatrix(), _scale(1), _offset(0) { fill(0); }
    Kernel(const int *values) : QGenericMatrix(values), _scale(1), _offset(0) {}

    int rows() const { return 5; }
    int columns() const { return 5; }

    bool isValid() const {
        const int *values = data();
        for (int i = 0; i < rows() * columns(); i++) {
            if (values[i] != 0)
                return true;
        }
        return false;
    }

    int scale() const { return _scale; }
    int offset() const { return _offset; }

    void setScale(int value) { _scale = value; }
    void setOffset(int value) { _offset = value; }

private:
    int _scale;
    int _offset;
};

#endif // KERNEL_HPP
