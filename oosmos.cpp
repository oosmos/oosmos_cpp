#include "oosmos.hpp"
#include <list>
#include <iostream>

using namespace std;

namespace OOSMOS {
  cObject::cObject()
  {
    m_ObjectList.push_back(this);
  }

  cObject::cStack::cStack()
  {
    m_ThreadContext          = OOSMOS_THREAD_CONTEXT_BEGIN;
    m_ThreadFunctionIsActive = false;

    ResetThreadTimeout();
  }

  void cObject::cStack::TimeoutInSeconds(cObject::cStack::sTimeout * pTimeout, uint32_t TimeoutSeconds) const
  {
    TimeoutInMS(pTimeout, TimeoutSeconds * 1000);
  }

  void cObject::cStack::TimeoutInMS(cObject::cStack::sTimeout * pTimeout, uint32_t TimeoutMS) const
  {
    TimeoutInUS(pTimeout, TimeoutMS * 1000);
  }

  void cObject::cStack::TimeoutInUS(cObject::cStack::sTimeout * pTimeout, uint32_t TimeoutUS) const
  {
    const uint32_t StartUS = OS::GetFreeRunningUS();

    pTimeout->m_StartUS   = StartUS;
    pTimeout->m_TimeoutUS = TimeoutUS;
  }

  bool cObject::cStack::IsThreadTimeoutActive() const
  {
    return (m_ThreadTimeout.m_StartUS != 0) || (m_ThreadTimeout.m_TimeoutUS != 0);
  }

  void cObject::cStack::ResetThreadTimeout()
  {
    m_ThreadTimeout.m_StartUS   = 0;
    m_ThreadTimeout.m_TimeoutUS = 0;
  }

  bool cObject::cStack::TimeoutHasExpired(const cObject::cStack::sTimeout * pTimeout) const
  {
    const uint32_t StartUS   = pTimeout->m_StartUS;
    const uint32_t TimeoutUS = pTimeout->m_TimeoutUS;
    const uint32_t NowUS     = OS::GetFreeRunningUS();

    return (NowUS - StartUS) >= TimeoutUS;
  }

  bool cObject::cStack::ThreadTimeoutMS(uint32_t MS)
  {
    if (IsThreadTimeoutActive()) {
      if (TimeoutHasExpired(&m_ThreadTimeout)) {
        ResetThreadTimeout();

        return true;
      }
    }
    else {
      TimeoutInMS(&m_ThreadTimeout, MS);
    }

    return false;
  }

  bool cObject::cStack::OOSMOS_ThreadDelayUS(uint32_t US)
  {
    if (IsThreadTimeoutActive()) {
      if (TimeoutHasExpired(&m_ThreadTimeout)) {
        ResetThreadTimeout();
        return true;
      }
    }
    else {
      TimeoutInUS(&m_ThreadTimeout, US);
    }

    return false;
  }

  bool cObject::cStack::OOSMOS_ThreadDelayMS(uint32_t MS)
  {
    if (IsThreadTimeoutActive()) {
      if (TimeoutHasExpired(&m_ThreadTimeout)) {
        ResetThreadTimeout();
        return true;
      }
    }
    else {
      TimeoutInMS(&m_ThreadTimeout, MS);
    }

    return false;
  }

  bool cObject::cStack::OOSMOS_ThreadWaitCond_TimeoutMS(bool Condition, uint32_t TimeoutMS, bool * pTimeoutStatus)
  {
    if (!m_ThreadFunctionIsActive) {
      ResetThreadTimeout();
      m_ThreadFunctionIsActive = true;
    }

    if (Condition) {
      *pTimeoutStatus = false;
      m_ThreadFunctionIsActive = false;
      return true;
    }

    if (ThreadTimeoutMS(TimeoutMS)) {
      *pTimeoutStatus = true;
      m_ThreadFunctionIsActive = false;
      return true;
    }

    return false;
  }

  bool cObject::cStack::OOSMOS_ThreadYield()
  {
    if (!m_ThreadFunctionIsActive) {
      m_ThreadFunctionIsActive = true;
      return false;
    }

    m_ThreadFunctionIsActive = false;
    return true;
  }

  void cObject::AssertWarn(bool MustBeTrue, const char * pMessage) const
  {
    if (!MustBeTrue) {
      cout << pMessage << endl;
    }
  }

  void Run(void) {
    for (auto pOOSMOS: m_ObjectList) {
      pOOSMOS->Run();
    }
  }

  list<cObject *> m_ObjectList;
}
