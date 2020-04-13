#include "oosmos.hpp"
#include "os.hpp"
#include <iostream>
#include <cstdint>

using namespace std;

struct cMyObject : public OOSMOS::cObject
{
  uint32_t m_ObjectMember = 0;

  struct cTestThreadStack : public OOSMOS::cStack
  {
    int   i;
    bool  TimedOut;
  } TestThread_Stack;

  void TestThread(cTestThreadStack& rStack)
  {
    ThreadBegin();
      for (rStack.i = 1; rStack.i <= 5; rStack.i++) {
        cout << "TestThread: Iteration " << rStack.i << endl;
        ThreadDelayUS(300);
      }

      cout << "TestThread: DelaySeconds" << endl;
      ThreadDelaySeconds(1);

      cout << "TestThread: Yield" << endl;
      ThreadYield();

      m_ObjectMember += 1;

      cout << "TestThread: WaitCond" << endl;
      ThreadWaitCond(true);

      cout << "TestThread: WaitCond_Timeout 1" << endl;
      ThreadWaitCond_TimeoutMS(true, 100, &rStack.TimedOut);
      AssertWarn(!rStack.TimedOut, "Should not have timed out.");

      cout << "TestThread: WaitCond_Timeout 2" << endl;
      ThreadWaitCond_TimeoutMS(false, 100, &rStack.TimedOut);
      AssertWarn(rStack.TimedOut, "Should have timed out.");

      cout << "TestThread: Exit (to ThreadFinally)" << endl;
      ThreadExit();
      cout << "TestThread: Should not get here" << endl;
    ThreadFinally();
      cout << "TestThread: Exiting" << endl;
    ThreadEnd();
  }

  void Run()
  {
    TestThread(TestThread_Stack);
  }
};

int main()
{
  cMyObject MyObject;

  for (;;) {
    OOSMOS::Run();
    OS::DelayMS(1);
  }
}
