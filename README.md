# OOSMOS for C++

OOSMOS is the Object-Oriented State Machine Operating System, which features support for generative hierarchical state machines, publish/subscribe event processing, and first-come first-served scheduling.  OOSMOS leverages [ProtoThreads ](http://dunkels.com/adam/pt/) to implement two scoped types of threading: state threads and object threads. See [OOSMOS for C](https://www.oosmos.com).

This C++ implementation is a specialized lite version of OOSMOS, supporting only object threads - concurrency for C++.

Here is an example usage:

```cpp
#include "oosmos.hpp"
#include <cstdint>
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

  struct cThreadA : public cTSS { // TSS - Thread Specific Storage
    int   i;
    bool  TimedOut;
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

## Building the Example

(Assumes that Python is installed.)

Open a command line window whose environment variables are set up for any version of Visual Studio C++, VS 2013 through 2019.

Then run:

```text
build.py
```

Which will result in the executable `thread_test.exe`.

Run `thread_test.exe` and you'll see the following output:

```text
On
Off
On
Off
On
Off
exiting
```

The program does not terminate.  You must press CNTL-C to exit.

Note that `build.py` compiles the program with debug enabled (`/Zi`).  Use the `debug.py` script to launch the Microsoft debugger to best learn how it works.
