# OOSMOS for C++


```cpp
#include "oosmos.hpp"
#include <iostream>

using namespace std;

struct cMyObject : public OOSMOS::cObject {
  cMyObject() : cObject() {
    m_BlinkCount = 0;
  }

  int m_BlinkCount;

  static void SetLED(bool On) {
    cout << (On ? "On" : "Off") << endl;
  }

  struct cThreadA : public cTSS {
    uint32_t i;
    bool     TimedOut;
  } ThreadA_Data;

  void ThreadA(cThreadA& rTSS) {
    ThreadBegin();
      for (rTSS.i = 1; rTSS.i <= 3; rTSS.i++) {
        SetLED(true);
        ThreadDelayMS(250);
        SetLED(false);
        ThreadDelayMS(750);

        m_BlinkCount += 1;
      }

      ThreadDelayUS(1000);
      ThreadDelaySeconds(1);
      ThreadYield();
      ThreadWaitCond(true);

      ThreadWaitCond_TimeoutMS(true, 100, &rTSS.TimedOut);
      AssertWarn(!rTSS.TimedOut, "Should not have timed out.");

      ThreadWaitCond_TimeoutMS(false, 100, &rTSS.TimedOut);
      AssertWarn(rTSS.TimedOut, "Should have timed out.");

      ThreadExit();
      cout << "should not get here" << endl;
    ThreadFinally();
      cout << "exiting" << endl;
    ThreadEnd();
  }

  void Run() {
   ThreadA(ThreadA_Data);
  }
};

void main(void) {
  cMyObject MyObject1;
  cMyObject MyObject2;

  for (;;) {
    OOSMOS::Run();
    OS::DelayMS(1);
  }
}

```
