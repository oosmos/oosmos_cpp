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

#include "oosmos.hpp"
#include <list>
#include <iostream>

using namespace std;

namespace OOSMOS {
  cObject::cObject()
  {
    m_ObjectList.push_back(this);
  }

  cObject::cTSS::cTSS()
  {
    m_ThreadContext          = OOSMOS_THREAD_CONTEXT_BEGIN;
    m_ThreadHasYielded       = false;
    m_ThreadFunctionIsActive = false;

    ResetThreadTimeout();
  }

  void cObject::cTSS::TimeoutInSeconds(cObject::cTSS::sTimeout * pTimeout, uint32_t TimeoutSeconds)
  {
    TimeoutInMS(pTimeout, TimeoutSeconds * 1000);
  }

  void cObject::cTSS::TimeoutInMS(cObject::cTSS::sTimeout * pTimeout, uint32_t TimeoutMS)
  {
    TimeoutInUS(pTimeout, TimeoutMS * 1000);
  }

  void cObject::cTSS::TimeoutInUS(cObject::cTSS::sTimeout * pTimeout, uint32_t TimeoutUS)
  {
    const uint32_t StartUS = OS::GetFreeRunningUS();

    pTimeout->m_StartUS   = StartUS;
    pTimeout->m_TimeoutUS = TimeoutUS;
  }

  bool cObject::cTSS::IsThreadTimeoutActive()
  {
    return (m_ThreadTimeout.m_StartUS != 0) || (m_ThreadTimeout.m_TimeoutUS != 0);
  }

  void cObject::cTSS::ResetThreadTimeout()
  {
    m_ThreadTimeout.m_StartUS   = 0;
    m_ThreadTimeout.m_TimeoutUS = 0;
  }

  bool cObject::cTSS::TimeoutHasExpired(const cObject::cTSS::sTimeout * pTimeout)
  {
    const uint32_t StartUS   = pTimeout->m_StartUS;
    const uint32_t TimeoutUS = pTimeout->m_TimeoutUS;
    const uint32_t NowUS     = OS::GetFreeRunningUS();

    return (NowUS - StartUS) >= TimeoutUS;
  }

  bool cObject::cTSS::ThreadTimeoutMS(uint32_t MS)
  {
    if (IsThreadTimeoutActive()) {
      if (TimeoutHasExpired(&m_ThreadTimeout)) {
        ResetThreadTimeout();

        //(void) DeliverEvent(&EventTIMEOUT);
        return true;
      }
    }
    else {
      TimeoutInMS(&m_ThreadTimeout, MS);
    }

    return false;
  }

  bool cObject::cTSS::OOSMOS_ThreadDelayUS(uint32_t US)
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

  bool cObject::cTSS::OOSMOS_ThreadDelayMS(uint32_t MS)
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

  bool cObject::cTSS::OOSMOS_ThreadWaitCond_TimeoutMS(bool Condition, uint32_t TimeoutMS, bool * pTimeoutStatus)
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

  bool cObject::cTSS::OOSMOS_ThreadYield()
  {
    if (!m_ThreadHasYielded) {
      m_ThreadHasYielded = true;
      return false;
    }

    m_ThreadHasYielded = false;
    return true;
  }

  void cObject::AssertWarn(bool MustBeTrue, const char * pMessage)
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