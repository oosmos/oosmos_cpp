#include "oosmos.hpp"
#include <iostream>

using namespace std;

struct cMyObject : public OOSMOS::cObject {
  cTSS BlinkingThread_Data;

  void BlinkingThread(cTSS& rTSS) {
    ThreadBegin();
      for (;;) {
        cout << "BlinkingThread: LED On" << endl;
        ThreadDelayMS(250);
        cout << "BlinkingThread: LED Off" << endl;
        ThreadDelayMS(750);
      }
    ThreadEnd();
  }

  cTSS BeepingThread_Data;

  uint32_t m_BeepCount = 0;

  void BeepingThread(cTSS& rTSS) {
    ThreadBegin();
      for (;;) {
        m_BeepCount += 1;
        cout << "BeepingThread: Beep " << m_BeepCount << endl;
        ThreadDelaySeconds(2);
      }
    ThreadEnd();
  }

  struct cTestThread : public cTSS {
    int   i;
    bool  TimedOut;
  } TestThread_Data;

  void TestThread(cTestThread& rTSS) {
    ThreadBegin();
      for (rTSS.i = 1; rTSS.i <= 5; rTSS.i++) {
        cout << "TestThread: Iteration " << rTSS.i << endl;
        ThreadDelayUS(300);
      }

      cout << "TestThread: DelaySeconds" << endl;
      ThreadDelaySeconds(1);

      cout << "TestThread: Yield" << endl;
      ThreadYield();

      cout << "TestThread: WaitCond" << endl;
      ThreadWaitCond(true);

      cout << "TestThread: WaitCond_Timeout1" << endl;
      ThreadWaitCond_TimeoutMS(true, 100, &rTSS.TimedOut);
      AssertWarn(!rTSS.TimedOut, "Should not have timed out.");

      cout << "TestThread: WaitCond_Timeout2" << endl;
      ThreadWaitCond_TimeoutMS(false, 100, &rTSS.TimedOut);
      AssertWarn(rTSS.TimedOut, "Should have timed out.");

      cout << "TestThread: Exit (to ThreadFinally)" << endl;
      ThreadExit();
      cout << "TestThread: Should not get here" << endl;
    ThreadFinally();
      cout << "TestThread: Exiting" << endl;
    ThreadEnd();
  }

  void Run() {
    TestThread(TestThread_Data);
    BlinkingThread(BlinkingThread_Data);
    BeepingThread(BeepingThread_Data);
  }
};

int main(void) {
  cMyObject MyObject;

  for (;;) {
    OOSMOS::Run();
    OS::DelayMS(1);
  }
}
