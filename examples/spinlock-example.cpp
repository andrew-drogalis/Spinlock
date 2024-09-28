// Andrew Drogalis Copyright (c) 2024, GNU 3.0 Licence
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include "dro/spinlock.hpp"

#include <thread>

int main(int argc, char* argv[])
{

  struct Data
  {
    int x;
  } data;

  const size_t iters {1'000};

  dro::Spinlock spinlock;

  auto thrd = std::thread([&] {
    for (int i {}; i < iters; ++i)
    {
      spinlock.lock();
      auto x = data.x;
      spinlock.unlock();
    }
  });

  for (int i {}; i < iters; ++i)
  {
    spinlock.lock();
    data.x = i;
    spinlock.unlock();
  }
  thrd.join();

  return 0;
}
