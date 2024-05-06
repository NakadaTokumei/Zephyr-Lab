#ifndef __KTHREAD_H__
#define __KTHREAD_H__

#include <zephyr/kernel.h>

namespace tkl
{

template<uint32_t THREAD_STACK_SIZE>
class kthread
{
    // Thread ID
    k_tid_t _tid;

    // Thread Object
    struct k_thread _thread;
    // Thread Stack
    k_thread_stack_t __aligned(Z_KERNEL_STACK_OBJ_ALIGN)
                     _tstack[Z_KERNEL_STACK_SIZE_ADJUST(THREAD_STACK_SIZE)];
public:
    kthread() : _tid(nullptr)
    { }

    template<typename T1 = void, typename T2 = void, typename T3 = void>
    bool Create(
        k_thread_entry_t threadFn,
        T1* arg1 = nullptr,
        T2* arg2 = nullptr,
        T3* arg3 = nullptr
    );

    void 
    Setname(
        const char* threadNames
    );

    void 
    Start(void);
    
    void
    Abort(void);

    uint32_t
    ThreadStatus(void);
};

template<uint32_t TSIZE>
template<typename T1, typename T2, typename T3>
bool kthread<TSIZE>::Create(
    k_thread_entry_t threadFn,
    T1* arg1,
    T2* arg2,
    T3* arg3
)
{
    bool ret = true;
    uint32_t threadStatus = ThreadStatus();

    if ((threadStatus == 0x00) || // First State
        (threadStatus & 0x08))    // Or Dead Now
    {
        _tid = k_thread_create(
            &_thread,
            reinterpret_cast<k_thread_stack_t*>(&_tstack),
            K_THREAD_STACK_SIZEOF(_tstack),
            threadFn,
            arg1,
            arg2,
            arg3,
            0,
            0,
            K_NO_WAIT
        );

        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}

template<uint32_t TSIZE>
void
kthread<TSIZE>::Setname(
    const char* threadName
)
{
    if(_tid != nullptr)
    {
        k_thread_name_set(_tid, threadName);
    }
}

template<uint32_t TSIZE>
void 
kthread<TSIZE>::Abort(void)
{
    k_thread_abort(_tid);
}

template<uint32_t TSIZE>
void
kthread<TSIZE>::Start(void)
{
    k_thread_start(_tid);
}

template<uint32_t TSIZE>
uint32_t
kthread<TSIZE>::ThreadStatus(void)
{
    return _thread.base.thread_state;
}

};
#endif