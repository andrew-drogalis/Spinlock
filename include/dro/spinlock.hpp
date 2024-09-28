// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef DRO_SPINLOCK
#define DRO_SPINLOCK

#include <atomic>// for std::atomic
#include <new>   // for std::hardware_destructive_interference_size

namespace dro
{

#ifdef __cpp_lib_hardware_interference_size
static constexpr std::size_t cacheLineSize =
    std::hardware_destructive_interference_size;
#else
static constexpr std::size_t cacheLineSize = 64;
#endif

class Spinlock
{
private:
  alignas(cacheLineSize) std::atomic<bool> lock_ {false};

public:
  void lock() noexcept
  {
    for (;;)
    {
      // Optimistically assume lock is free
      if (! lock_.exchange(true, std::memory_order_acquire))
      {
        return;
      }
      // Wait for lock to be released and minimize cache misses
      while (lock_.load(std::memory_order_relaxed)) {}
    }
  }

  bool try_lock() noexcept
  {
    // Relaxed load prevents unnecessary cacke misses
    return ! lock_.load(std::memory_order_relaxed) &&
           ! lock_.exchange(true, std::memory_order_acquire);
  }

  void unlock() noexcept { lock_.store(false, std::memory_order_release); }
};
}// namespace dro
#endif
