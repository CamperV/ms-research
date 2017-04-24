import sys
import csv

f = open(sys.argv[1], 'rb')
reader = csv.reader(f)
for line in csv.reader(f):
  print "\hline"
  for item in line:
    if line.index(item) == len(line)-1:
      print item + " \\\\"
    else:
      print item + " & ",

f.close()
