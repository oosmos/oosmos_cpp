#ifndef OOSMOS_HPP
#define OOSMOS_HPP

#include <cstdint>
#include <list>
#include "os.hpp"

using namespace std;

namespace OOSMOS {
  struct cStack {
    cStack();

    struct sTimeout {
      uint32_t m_StartUS;
      uint32_t m_TimeoutUS;
    };

    int32_t  m_ThreadContext;
    bool     m_ThreadFunctionIsActive;
    sTimeout m_ThreadTimeout;

    void TimeoutInSeconds(sTimeout * pTimeout, uint32_t TimeoutSeconds) const;
    void TimeoutInMS(sTimeout * pTimeout, uint32_t TimeoutMS) const;
    void TimeoutInUS(sTimeout * pTimeout, uint32_t TimeoutUS) const;

    bool IsThreadTimeoutActive() const;
    void ResetThreadTimeout();
    bool TimeoutHasExpired(const sTimeout * pTimeout) const;
    bool ThreadTimeoutMS(uint32_t MS);

    bool OOSMOS_ThreadDelayUS(uint32_t US);
    bool OOSMOS_ThreadDelayMS(uint32_t MS);
    bool OOSMOS_ThreadWaitCond_TimeoutMS(bool Condition, uint32_t TimeoutMS, bool * pTimeoutStatus);
    bool OOSMOS_ThreadYield();

    #define OOSMOS_THREAD_CONTEXT_BEGIN   (-1)
    #define OOSMOS_THREAD_CONTEXT_FINALLY (-2)
    #define OOSMOS_THREAD_CONTEXT_END     (-3)

    #define ThreadBegin() \
                                        switch (rStack.m_ThreadContext) { \
                                          case OOSMOS_THREAD_CONTEXT_BEGIN:
    #define ThreadDelayUS(US) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadDelayUS(US)) \
                                              return; \
                                          } while (0)

    #define ThreadDelayMS(MS) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadDelayMS(MS)) \
                                              return; \
                                          } while(0)

    #define ThreadDelaySeconds(Seconds) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadDelayMS(Seconds * 1000)) \
                                              return; \
                                          } while(0)

    #define ThreadYield() \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadYield()) \
                                              return; \
                                          } while(0)

    #define ThreadWaitCond(Cond) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!(Cond)) \
                                              return; \
                                          } while(0)

    #define ThreadWaitCond_TimeoutMS(Cond, TimeoutMS, pTimeoutStatus) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadWaitCond_TimeoutMS(Cond, TimeoutMS, pTimeoutStatus)) \
                                              return; \
                                          } while(0)

    #define ThreadExit() \
                                          do { rStack.m_ThreadContext = OOSMOS_THREAD_CONTEXT_FINALLY; \
                                            return; \
                                          } while(0)

    #define ThreadFinally() \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          do { case OOSMOS_THREAD_CONTEXT_FINALLY: rStack.m_ThreadContext = OOSMOS_THREAD_CONTEXT_END; \
                                          } while (0)

    #define ThreadEnd() \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          default:  \
                                            rStack.m_ThreadContext = OOSMOS_THREAD_CONTEXT_END; \
                                        } return
  };

  struct cObject {
    cObject();

    void AssertWarn(bool, const char *) const;

    virtual void Run(void) = 0;
  };

  extern void Run(void);

  extern list<struct cObject *> m_ObjectList;
}

#endif
