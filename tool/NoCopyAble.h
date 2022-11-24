#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

class NoCopyAble
{
public:
    NoCopyAble(const NoCopyAble &) = delete;
    NoCopyAble &operator=(const NoCopyAble &) = delete;

protected:
    NoCopyAble() = default;
    ~NoCopyAble() = default;
};

#endif
