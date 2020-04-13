#ifndef OOSMOS_HPP
#define OOSMOS_HPP

#include <cstdint>
#include <list>
#include "os.hpp"

using namespace std;

namespace OOSMOS {
  struct cTimeout {
    cTimeout();

    void TimeoutInSeconds(uint32_t TimeoutSeconds);
    void TimeoutInMS(uint32_t TimeoutMS);
    void TimeoutInUS(uint32_t TimeoutUS);
    bool HasExpired() const;

    uint32_t m_StartUS;
    uint32_t m_TimeoutUS;
  };

  struct cStack {
    cStack();

    int32_t  m_ThreadContext;
    bool     m_FirstEntry;
    cTimeout m_ThreadTimeout;

    bool OOSMOS_ThreadDelayUS(uint32_t US);
    bool OOSMOS_ThreadDelayMS(uint32_t MS);
    bool OOSMOS_ThreadDelaySeconds(uint32_t Seconds);
    bool OOSMOS_ThreadWaitCond_TimeoutMS(bool Condition, uint32_t TimeoutMS, bool * pTimeoutStatus);
    bool OOSMOS_ThreadYield();

    #define OOSMOS_THREAD_CONTEXT_BEGIN   (-1)
    #define OOSMOS_THREAD_CONTEXT_FINALLY (-2)
    #define OOSMOS_THREAD_CONTEXT_END     (-3)

    // The underlying structure of OOSMOS C++ State Threads and Object Threads is based
    // on protothreads by Adam Dunkels.

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
                                          } while (0)

    #define ThreadDelaySeconds(Seconds) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadDelaySeconds(Seconds)) \
                                              return; \
                                          } while (0)

    #define ThreadYield() \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!rStack.OOSMOS_ThreadYield()) \
                                              return; \
                                          } while (0)

    #define ThreadWaitCond(Cond) \
                                          /*lint -e646 suppress "case/default within for loop; may have been misplaced" */ \
                                          /*lint -fallthrough*/ \
                                          do { case __LINE__: rStack.m_ThreadContext = __LINE__; \
                                            if (!(Cond)) \
                                              return; \
                                          } while (0)

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
                                          } while (0)

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
