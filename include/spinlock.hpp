
#ifndef SPINLOCK
#define SPINLOCK

#include <atomic>
namespace dro {

struct Spinlock {
    std::atomic<bool> lock_ {0};

    void lock() noexcept {
        for (;;)
        {
            // Optimistically assume lock is free
            if (!lock_.exchange(true, std::memory_order_acquire)) {
                return;
            }
            // Wait for lock to be released and minimize cache misses
            while (lock_.load(std::memory_order_relaxed)) {
                // Issue X86 PAUSE or ARM YIELD instruction to reduce contention
                // between hyper-threads
                __builtin_ia32_pause();
            } 
        }    
    }

    bool try_lock() noexcept {
        // Relaxed load prevents unnecessary cacke misses
        return !lock_.load(std::memory_order_relaxed) &&
               !lock_.exchange(true, std::memory_order_acquire); 
    }

    void unlock() noexcept {
        lock_.store(false, std::memory_order_release);
    }
}; 
}
#endif
