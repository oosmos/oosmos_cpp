#include "oosmos.hpp"
#include <iostream>

using namespace std;

struct cMyObject : public OOSMOS::cObject {
  cStack BlinkingThread_Stack;

  void BlinkingThread(cStack& rStack) {
    ThreadBegin();
      for (;;) {
        cout << "BlinkingThread: LED On" << endl;
        ThreadDelayMS(250);
        cout << "BlinkingThread: LED Off" << endl;
        ThreadDelayMS(750);
      }
    ThreadEnd();
  }

  cStack BeepingThread_Stack;

  uint32_t m_BeepCount = 0;

  void BeepingThread(cStack& rStack) {
    ThreadBegin();
      for (;;) {
        m_BeepCount += 1;
        cout << "BeepingThread: Beep " << m_BeepCount << endl;
        ThreadDelaySeconds(2);
      }
    ThreadEnd();
  }

  struct cTestThreadStack : public cStack {
    int   i;
    bool  TimedOut;
  } TestThread_Stack;

  void TestThread(cTestThreadStack& rStack) {
    ThreadBegin();
      for (rStack.i = 1; rStack.i <= 5; rStack.i++) {
        cout << "TestThread: Iteration " << rStack.i << endl;
        ThreadDelayUS(300);
      }

      cout << "TestThread: DelaySeconds" << endl;
      ThreadDelaySeconds(1);

      cout << "TestThread: Yield" << endl;
      ThreadYield();

      cout << "TestThread: WaitCond" << endl;
      ThreadWaitCond(true);

      cout << "TestThread: WaitCond_Timeout1" << endl;
      ThreadWaitCond_TimeoutMS(true, 100, &rStack.TimedOut);
      AssertWarn(!rStack.TimedOut, "Should not have timed out.");

      cout << "TestThread: WaitCond_Timeout2" << endl;
      ThreadWaitCond_TimeoutMS(false, 100, &rStack.TimedOut);
      AssertWarn(rStack.TimedOut, "Should have timed out.");

      cout << "TestThread: Exit (to ThreadFinally)" << endl;
      ThreadExit();
      cout << "TestThread: Should not get here" << endl;
    ThreadFinally();
      cout << "TestThread: Exiting" << endl;
    ThreadEnd();
  }

  void Run() {
    TestThread(TestThread_Stack);
    BlinkingThread(BlinkingThread_Stack);
    BeepingThread(BeepingThread_Stack);
  }
};

int main() {
  cMyObject MyObject;

  for (;;) {
    OOSMOS::Run();
    OS::DelayMS(1);
  }
}
