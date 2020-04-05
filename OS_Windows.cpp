#include <windows.h>
#include <cstdint>

namespace OS {
  void DelayMS(uint32_t MS) {
    Sleep(1);
  }

  uint32_t GetFreeRunningUS(void) {
    SYSTEMTIME st;
    GetSystemTime(&st);

    uint64_t MS = 0;
    MS += st.wMilliseconds;
    MS += st.wSecond * 1000ULL;
    MS += st.wMinute * 60000ULL;
    MS += st.wHour   * 3600000ULL;

    const uint64_t US = MS * 1000;

    return (uint32_t) US;
  }
}