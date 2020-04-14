#include "oosmos.hpp"
#include "os.hpp"
#include <iostream>

using namespace std;

static void HeartbeatThread(OOSMOS::cStack& rStack)
{
  ThreadBegin();
    for (;;) {
      cout << "HeartbeatThread: Heartbeat On" << endl;
      ThreadDelayMS(50);
      cout << "HeartbeatThread: Heartbeat Off" << endl;
      ThreadDelaySeconds(2);
    }
  ThreadEnd();
}

int main()
{
  OOSMOS::cStack HeartbeatThread_Stack;

  for (;;) {
    HeartbeatThread(HeartbeatThread_Stack);
    OS::DelayMS(1);
  }
}
