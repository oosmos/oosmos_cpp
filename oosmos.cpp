#include "oosmos.hpp"
#include <list>
#include <iostream>

using namespace std;

namespace OOSMOS {
  cObject::cObject()
  {
    m_ObjectList.push_back(this);
  }

  cTimeout::cTimeout()
  {
    m_StartUS = 0;
    m_TimeoutUS = 0;
  }

  void OOSMOS::cTimeout::TimeoutInUS(uint32_t TimeoutUS)
  {
    m_StartUS   = OS::GetFreeRunningUS();
    m_TimeoutUS = TimeoutUS;
  }

  void OOSMOS::cTimeout::TimeoutInMS(uint32_t TimeoutMS)
  {
    TimeoutInUS(TimeoutMS * 1000);
  }

  void OOSMOS::cTimeout::TimeoutInSeconds(uint32_t TimeoutSeconds)
  {
    TimeoutInMS(TimeoutSeconds * 1000);
  }

  bool OOSMOS::cTimeout::HasExpired() const
  {
    const uint32_t NowUS = OS::GetFreeRunningUS();

    return (NowUS - m_StartUS) >= m_TimeoutUS;
  }

  OOSMOS::cStack::cStack()
  {
    m_ThreadContext = OOSMOS_THREAD_CONTEXT_BEGIN;
    m_FirstEntry    = true;
  }

  bool OOSMOS::cStack::OOSMOS_ThreadDelayUS(uint32_t US)
  {
    if (m_FirstEntry) {
      m_ThreadTimeout.TimeoutInUS(US);
      m_FirstEntry = false;
      return false;
    }

    if (m_ThreadTimeout.HasExpired()) {
      m_FirstEntry = true;
      return true;
    }

    return false;
  }

  bool OOSMOS::cStack::OOSMOS_ThreadDelayMS(uint32_t MS)
  {
    return OOSMOS_ThreadDelayUS(MS * 1000);
  }

  bool OOSMOS::cStack::OOSMOS_ThreadDelaySeconds(uint32_t Seconds)
  {
    return OOSMOS_ThreadDelayUS(Seconds * 1000 * 1000);
  }

  bool OOSMOS::cStack::OOSMOS_ThreadWaitCond_TimeoutMS(bool Condition, uint32_t TimeoutMS, bool * pTimeoutStatus)
  {
    if (m_FirstEntry) {
      m_ThreadTimeout.TimeoutInMS(TimeoutMS);
      m_FirstEntry = false;
      return false;
    }

    if (Condition) {
      *pTimeoutStatus = false;
      m_FirstEntry = true;
      return true;
    }

    if (m_ThreadTimeout.HasExpired()) {
      *pTimeoutStatus = true;
      m_FirstEntry = true;
      return true;
    }

    return false;
  }

  bool OOSMOS::cStack::OOSMOS_ThreadYield()
  {
    if (m_FirstEntry) {
      m_FirstEntry = false;
      return false;
    }

    m_FirstEntry = true;
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
