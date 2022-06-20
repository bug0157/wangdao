#ifndef __NOCOPYABLE_H__
#define __NOCOPYABLE_H__

class NoCopyable
{
protected:
    NoCopyable();
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable &operator=(const NoCopyable &) = delete;
    ~NoCopyable();
};


#endif // !__NOCOPYABLE_H__
