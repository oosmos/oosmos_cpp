#!/usr/bin/env python

import os
import platform

system = platform.system() 

if system == 'Windows':
    os.system('cl /W4 /Zi /EHsc static_threads.cpp oosmos.cpp os_windows.cpp')
    os.system('cl /W4 /Zi /EHsc object_threads.cpp oosmos.cpp os_windows.cpp')
    os.system('cl /W4 /Zi /EHsc -wd4127 -wd4702 test_threads.cpp oosmos.cpp os_windows.cpp')
elif system == 'Linux':
    os.system('g++ -o static_threads static_threads.cpp oosmos.cpp os_linux.cpp')
    os.system('g++ -o object_threads object_threads.cpp oosmos.cpp os_linux.cpp')
    os.system('g++ -o test_threads test_threads.cpp oosmos.cpp os_linux.cpp')
