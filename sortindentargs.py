#!/usr/bin/python
"""Sort indent argument lists.

This sorts indent argument lists so that the -n* arguments appear in
the same order as their -* counterparts.
"""
import sys

def argkey(arg):
    return arg.replace('-n', '-')

for argfile in sys.argv[1:]:
  lines = open(argfile).readlines()
  lines.sort(key=argkey)
  open(argfile, 'w').write(''.join(lines))
