//
// OOSMOS for C++ - The Object-Oriented State Machine Operating System
//
// Copyright (C) 2014-2020  OOSMOS, LLC
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2 of the License ("GPLv2").
//
// This software may be used without the GPLv2 restrictions by entering
// into a commercial license agreement with OOSMOS, LLC.
// See <https://www.oosmos.com/licensing/>.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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