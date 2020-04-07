# OOSMOS for C++

OOSMOS is the Object-Oriented State Machine Operating System, which features support for generative hierarchical state machines, publish/subscribe event processing, and first-come first-served scheduling.  OOSMOS leverages [ProtoThreads](http://dunkels.com/adam/pt/) to implement two scoped types of threading: state threads and object threads. See [OOSMOS for C](https://www.oosmos.com).

This C++ implementation is a specialized lite, MIT licensed, version of OOSMOS that supports only object threads - concurrency for C++.

Here is an example usage:

```cpp
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

  void BeepingThread(cTSS& rTSS) {
    ThreadBegin();
      for (;;) {
        cout << "BeepingThread: Beep" << endl;
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

void main(void) {
  cMyObject MyObject;

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

## Running the Example

Run `thread_test.exe` and you'll see the following output:

```text
TestThread: Iteration 1
BlinkingThread: LED On
BeepingThread: Beep
TestThread: Iteration 2
TestThread: Iteration 3
TestThread: Iteration 4
TestThread: Iteration 5
TestThread: DelaySeconds
BlinkingThread: LED Off
BlinkingThread: LED On
TestThread: Yield
TestThread: WaitCond
TestThread: WaitCond_Timeout1
TestThread: WaitCond_Timeout2
TestThread: Exit (to ThreadFinally)
TestThread: Exiting
BlinkingThread: LED Off
BeepingThread: Beep
BlinkingThread: LED On
BlinkingThread: LED Off
BlinkingThread: LED On
BlinkingThread: LED Off
BeepingThread: Beep
BlinkingThread: LED On
BlinkingThread: LED Off
BlinkingThread: LED On
BlinkingThread: LED Off
BeepingThread: Beep
...
```

The program does not terminate.  You must press CNTL-C to exit.

## How ProtoThreads Work

For a detailed walk-through of how ProtoThreads work, visit [HOW-PROTOTHREADS-WORK.md](HOW-PROTOTHREADS-WORK.md).

## Debugging

Note that `build.py` compiles the program with debug enabled (`/Zi`).  Use the `debug.py` script to launch the Microsoft debugger to step through an execution.
