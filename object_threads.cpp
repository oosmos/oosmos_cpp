#include "oosmos.hpp"
#include <iostream>

using namespace std;

struct cMyObject : public OOSMOS::cObject {
  OOSMOS::cStack BlinkingThread_Stack;

  void BlinkingThread(OOSMOS::cStack& rStack)
  {
    ThreadBegin();
      for (;;) {
        cout << "BlinkingThread: LED On" << endl;
        ThreadDelayMS(250);
        cout << "BlinkingThread: LED Off" << endl;
        ThreadDelayMS(750);
      }
    ThreadEnd();
  }

  OOSMOS::cStack BeepingThread_Stack;

  uint32_t m_BeepCount = 0;

  void BeepingThread(OOSMOS::cStack& rStack)
  {
    ThreadBegin();
      for (;;) {
        m_BeepCount += 1;
        cout << "BeepingThread: Beep " << m_BeepCount << endl;
        ThreadDelaySeconds(2);
      }
    ThreadEnd();
  }

  void Run()
  {
    BlinkingThread(BlinkingThread_Stack);
    BeepingThread(BeepingThread_Stack);
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
