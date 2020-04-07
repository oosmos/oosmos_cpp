#ifndef OS_HPP
#define OS_HPP

#include <cstdint>

namespace OS {
  void DelayMS(uint32_t MS);

  uint32_t GetFreeRunningUS(void);
}

#endif