#!/usr/bin/env python

import os
import platform

system = platform.system() 

if system == 'Windows':
    os.system('cl /Zi /EHsc thread_test.cpp oosmos.cpp os_windows.cpp')
elif system == 'Linux':
    os.system('g++ -o thread_test thread_test.cpp oosmos.cpp os_linux.cpp')
