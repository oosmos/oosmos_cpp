# OOSMOS for C++

OOSMOS is the Object-Oriented State Machine Operating System, which features support for generative hierarchical state machines, publish/subscribe event processing, and first-come first-served scheduling.  OOSMOS leverages [ProtoThreads](http://dunkels.com/adam/pt/) to implement two scoped types of threading: state threads and object threads. See [OOSMOS for C](https://www.oosmos.com).

This C++ implementation is a specialized lite version of OOSMOS that supports static threads and object threads - concurrency for C++. MIT licensed.

## Examples

### Object Threads Example

Here is an example of two object threads, where the thread functions are inside the scope of an object and that can access all the members of the containing object:

```cpp
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
```

### Static Thread Example

Here is an example of a static thread at file scope:

```cpp
#include "oosmos.hpp"
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
```

## Stack Variables

One of the notable and severe limitations of the original C implementation of ProtoThreads is that any values stored in variables on the runtime stack are not preserved from one invocation of the thread function to the next. This implementation addresses this limitation by introducting a stack per thread object where the programmer can store values from one invocation to the next in a clean, reliable, and readable way.  Call it a ProtoStack.

In the example below, see how the `TestThread` function uses the members of the `cTestThreadStack` class.

```cpp
#include "oosmos.hpp"
#include <iostream>

using namespace std;

struct cMyObject : public OOSMOS::cObject {
  uint32_t m_ObjectMember = 0;

  struct cTestThreadStack : public OOSMOS::cStack {
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
```

## Building the Examples

(Assumes that Python is installed.)

Open a command line window whose environment variables are set up for any version of Visual Studio C++, VS 2013 through 2019.

Then run:

```text
build.py
```

This will build three example applications: `object_threads.exe`, `static_threads.exe`, and `test_threads.exe`.

## Running the `test_threads` Example

Run `test_threads.exe` and you'll see the following output:

```text
TestThread: Iteration 1
TestThread: Iteration 2
TestThread: Iteration 3
TestThread: Iteration 4
TestThread: Iteration 5
TestThread: DelaySeconds
TestThread: Yield
TestThread: WaitCond
TestThread: WaitCond_Timeout1
TestThread: WaitCond_Timeout2
TestThread: Exit (to ThreadFinally)
TestThread: Exiting
...
```

The program does not terminate.  You must press `CNTL-C` to exit.

## How ProtoThreads Work

For a detailed walk-through of how ProtoThreads work, visit [HOW-PROTOTHREADS-WORK.md](HOW-PROTOTHREADS-WORK.md).

## Rules

1. You must call each thread function periodically.
   * If you use object threads, you must override virtual function `Run()` in each object that you create and then call each thread function, in turn.
   * On Windows and Linux, you'll want to throttle the calls to thread functions with a hard delay in order to be polite to others on the system. See `OS::DelayMS(1)` in the example.  Vary the delay time depending on how responsive you need your application to be.
   * On 'bare metal' (Arduino, PIC32, STM32, etc.), you'll want to run without throttling.
2. You must allocate a stack for each thread.
   * If you have iterators or need other variables local to the function, you must specialize `OOSMOS::cStack` and allocate them there (see `cTestThread` in the example).
   * If you don't need local variables, then simply allocate a stack of type `cStack`. See `BeepingThread_Stack` in the example.
3. You must pass at least one argument to each thread function that is a reference to the thread's stack that you created.  The name of the argument _must_ be `rStack`.
4. For new platforms, implement a new `os_<name>.cpp` file that conforms to the modest interface specified in `os.hpp`.
