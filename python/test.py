import combalg
import time

  
hist = {}
n = 7
for x in combalg.partitions(n):
  hist[tuple(x)] = 0
print
for i in xrange(15000):
  t = tuple(combalg.random_partition(n))
#  print i,t
  hist[t] += 1
for x in sorted(hist.keys(), reverse=True):
  print x,hist[x]

  
# 0, 1, 2, 3, 4, 5,  6,  7,  8,  9, 10, 11, 12,  13,  14,  15,  16,  17,  18,  19,  20
# 1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42, 56, 77, 101, 135, 176, 231, 297, 385, 490, 627


# (3), (2,1), (1,1,1)
