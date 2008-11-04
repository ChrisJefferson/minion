# First attempt at a benchmarking script
import sys, os, timeit
program = sys.argv[1]
benchmarks = sys.argv[2]
for root, dirs, files in os.walk(benchmarks):
  for name in files:
    if name.endswith('.minion'):
      timer = timeit.Timer('os.system("'+program+' '+root+'/'+name+' &> /dev/null")','import os')
      time = timer.timeit(1)
      if(time < 5.0):
        # This is a fast experiment
        time = timer.repeat(3,5)
        mintime = min(time)/10.0
      else:
        time = timer.repeat(3,1)
        mintime = min(time)
      FILE = open(name + ".benchmark", "w")
      FILE.write('YVALUE=%.3f' % mintime)
      FILE.close