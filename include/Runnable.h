#ifndef RUNNABLE_H
#define RUNNABLE_H

namespace iu
{
    class Runnable
    {
    public:
        Runnable() = default;
        virtual ~Runnable() = default;
        virtual void operator()() = 0;
    };
}

#endif //RUNNABLE_H