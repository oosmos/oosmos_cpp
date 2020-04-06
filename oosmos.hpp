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

#ifndef OOSMOS_HPP
#define OOSMOS_HPP

#include <cstdint>
#include <list>
#include "os.hpp"

using namespace std;

namespace OOSMOS {
  struct cObject {
    cObject();

    struct cTSS {
      cTSS();

      struct sTimeout {
        uint32_t m_StartUS;
        uint32_t m_TimeoutUS;
      };

      uint32_t m_ThreadContext;
      bool     m_ThreadHasYielded;
      bool     m_ThreadFunctionIsActive;
      sTimeout m_ThreadTimeout;

      void TimeoutInSeconds(sTimeout * pTimeout, uint32_t TimeoutSeconds);
      void TimeoutInMS(sTimeout * pTimeout, uint32_t TimeoutMS);
      void TimeoutInUS(sTimeout * pTimeout, uint32_t TimeoutUS);

      bool IsThreadTimeoutActive();
      void ResetThreadTimeout();
      bool TimeoutHasExpired(const sTimeout * pTimeout);
      bool ThreadTimeoutMS(uint32_t MS);

      bool OOSMOS_ThreadDelayUS(uint32_t US);
      bool OOSMOS_ThreadDelayMS(uint32_t MS);
      bool OOSMOS_ThreadWaitCond_TimeoutMS(bool Condition, uint32_t TimeoutMS, bool * pTimeoutStatus);
      bool OOSMOS_ThreadYield();

      #define OOSMOS_THREAD_CONTEXT_BEGIN   (-1)
      #define OOSMOS_THREAD_CONTEXT_FINALLY (-2)
      #define OOSMOS_THREAD_CONTEXT_END     (-3)

      #define ThreadBegin() \
                                          switch (rTSS.m_ThreadContext) { \
                                            case OOSMOS_THREAD_CONTEXT_BEGIN:
      #define ThreadDelayUS(US) \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            case __LINE__: rTSS.m_ThreadContext = __LINE__; \
                                              if (!rTSS.OOSMOS_ThreadDelayUS(US)) \
                                                return

      #define ThreadDelayMS(MS) \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            case __LINE__: rTSS.m_ThreadContext = __LINE__; \
                                              if (!rTSS.OOSMOS_ThreadDelayMS(MS)) \
                                                return

      #define ThreadDelaySeconds(Seconds) \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            case __LINE__: rTSS.m_ThreadContext = __LINE__; \
                                              if (!rTSS.OOSMOS_ThreadDelayMS(Seconds * 1000)) \
                                                return

      #define ThreadYield() \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            case __LINE__: rTSS.m_ThreadContext = __LINE__; \
                                              if (!rTSS.OOSMOS_ThreadYield()) \
                                                return

      #define ThreadWaitCond(Cond) \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            case __LINE__: rTSS.m_ThreadContext = __LINE__; \
                                              if (!(Cond)) \
                                                return

      #define ThreadWaitCond_TimeoutMS(Cond, TimeoutMS, pTimeoutStatus) \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            case __LINE__: rTSS.m_ThreadContext = __LINE__; \
                                              if (!rTSS.OOSMOS_ThreadWaitCond_TimeoutMS(Cond, TimeoutMS, pTimeoutStatus)) \
                                                return

      #define ThreadExit() \
                                              rTSS.m_ThreadContext = OOSMOS_THREAD_CONTEXT_FINALLY; \
                                              return

      #define ThreadFinally() \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            case OOSMOS_THREAD_CONTEXT_FINALLY: rTSS.m_ThreadContext = OOSMOS_THREAD_CONTEXT_END

      #define ThreadEnd() \
                                            /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                            /*lint -fallthrough*/ \
                                            default:  \
                                              rTSS.m_ThreadContext = OOSMOS_THREAD_CONTEXT_END; \
                                          } return
    };

    void AssertWarn(bool, const char *);

    virtual void Run(void) = 0;
  };

  extern void Run(void);

  extern list<struct cObject *> m_ObjectList;
}

#endif
