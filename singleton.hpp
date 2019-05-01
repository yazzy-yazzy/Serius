#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T>
class Singleton
{
    friend T;

public:
    static T &instance();

private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
};

template <typename T>
T& Singleton<T>::instance()
{
    static T inst;
    return inst;
}

#endif // SINGLETON_HPP
