# How ProtoThreads Work

## Overview

The `ProtoThread` 'trick' is to use the C++ preprocessor to textually replace the `OOSMOS` `Thread` APIs with `switch`, `case` and `default` statements.

Each thread function is continuously entered, and then exited when it is determined that an asynchronous operation cannot be completed during this execution.  Each thread function is then again reentered, picking up where it left off the last time until the current asynchronous operation is complete, when it then drops down to subsequent statements where it may encounter another asynchronous operation (an `OOSMOS` `Thread` function) and the process repeats.

## Detailed Walkthrough

For reference:

```cpp
#define OOSMOS_THREAD_CONTEXT_BEGIN (-1)
#define OOSMOS_THREAD_CONTEXT_END   (-3)
```

```cpp
__LINE__ Original source code                              Preprocessor output
======== ================================================= ========================================
       2                                                   #line 3 "thread_test.cpp"
       3
       4 using namespace std;                              using namespace std;
       5
       6 struct cMyObject : public OOSMOS::cObject {       struct cMyObject : public OOSMOS::cObject {
       7   cTSS BlinkingThread_Data;                         cTSS BlinkingThread_Data;
       8
       9   void BlinkingThread(cTSS& rTSS) {                 void BlinkingThread(cTSS& rTSS) {
      10     ThreadBegin();                                    switch (rTSS.m_ThreadContext) { case (-1):;
      11       for (;;) {                                        for (;;) {
      12         cout << "BlinkingThread: LED On" << endl;         cout << "BlinkingThread: LED On" << endl;
      13         ThreadDelayMS(250);                               case 13: rTSS.m_ThreadContext = 13; if (!rTSS.OOSMOS_ThreadDelayMS(250)) return;
      14         cout << "BlinkingThread: LED Off" << endl;        cout << "BlinkingThread: LED Off" << endl;
      15         ThreadDelayMS(750);                               case 15: rTSS.m_ThreadContext = 15; if (!rTSS.OOSMOS_ThreadDelayMS(750)) return;
      16       }                                                 }
      17     ThreadEnd();                                      default: rTSS.m_ThreadContext = (-3); } return;
      18   }                                                 }
```

* On line 2: the preprocessor synchronizes line numbers, indicating that #line 3 is the next line.

* On line 10, the `ThreadBegin()` macro is replaced with the `switch` and `case` statement. When the thread was started, `rTSS.m_ThreadContext` was initialized to `OOSMOS_THREAD_CONTEXT_BEGIN` which is `-1`, so, on entry to `BlinkingThread`, the `switch` first executes `case (-1)` and then drops into the `for` statement. (No `break` statement.)

* Line 11, the `for` statement, is executed.

* Line 12, the `cout` statement is executed and then drops into line 13. (No `break` statement.)

* Line 13, the `ThreadDelayMS` statement is executed. Note that `rTSS.m_ThreadContext` is set to 13 (via special preprocessor symbol `__LINE__`), which is the current line number.  We then call the internal `OOSMOS` function `OOSMOS_ThreadDelayMS(250)` which will return `true` if the delay has expired.  If it has, then it will drop through to line 14 (no `break` statement).  If it has not, then the `return` statement causes an immediate return from the `BlinkingThread` function. But recall that we remembered line 13, the current line, in the `rTSS.m_ThreadContext` variable.  Therefore, the next time we enter `BlinkingThread` and execute the `switch` statement, we will come right back to line 13 again to check if the `ThreadDelayMS` has expired.  We continue to `return` and reenter `BlinkingThread` until the delay has expired.  This line alone demonstrates the magic of the [ProtoThread](http://dunkels.com/adam/pt/) technique.

* Line 14 is the next `cout` statement.  Nothing special here.

* Line 15 is handled just like line 13.

* Line 17, `rTSS.m_ThreadContext` is set to `-3`, so the next time we execute the `BlinkingThread` and the `switch` statement, it will continue to execute the `default` case, effectively ending the thread.
