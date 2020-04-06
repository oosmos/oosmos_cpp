#!/usr/bin/env python

import os

def Remove(Filename):
    os.system('rm -f %s' % Filename)

Remove('*.exe')
Remove('*.obj')
Remove('*.ilk')
Remove('*.pdb')
Remove('*.sln')
