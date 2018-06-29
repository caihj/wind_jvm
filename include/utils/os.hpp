/*
 * os.hpp
 *
 *  Created on: 2017年11月30日
 *      Author: zhengxiaolin
 */

#ifndef INCLUDE_UTILS_OS_HPP_
#define INCLUDE_UTILS_OS_HPP_

#ifdef __cplusplus
#include <atomic>
using namespace std;
#else
#include <stdatomic.h>
#endif

int get_cpu_nums();

// CAS, from x86 assembly (e.g. linux kernel), and this code is from openjdk.
inline int cmpxchg(int exchange_value, volatile int *dest, int compare_value)
{
    return __sync_val_compare_and_swap(dest,compare_value,exchange_value);
}

inline long cmpxchg(long exchange_value, volatile long *dest, long compare_value)
{
    return __sync_val_compare_and_swap(dest,compare_value,exchange_value);
}

inline void fence()
{
	// barrier. I use boost::atomic//ops_gcc_x86.hpp's method.
	// openjdk says `mfence` is always expensive...
    atomic_thread_fence(memory_order_seq_cst);
}

// from openjdk: OrderAccess
inline void release()
{
    atomic_thread_fence(memory_order_release);
}

// from openjdk: OrderAccess
inline void acquire()
{
    atomic_thread_fence(memory_order_acquire);
}


#endif /* INCLUDE_UTILS_OS_HPP_ */
