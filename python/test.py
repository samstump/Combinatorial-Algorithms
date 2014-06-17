import combalg
import time
 
def next_partition(n):
  r = [0]*n
  m = [0]*n
  r[0] = n
  m[0] = 1
  d = 1
  yield filter(lambda x: x > 0, r)
  while m[d-1] != n:
    if r[d-1] == 1:
      sigma = m[d-1] + 1
      d -= 1
    else:
      sigma = 1
    f = r[d-1] - 1
    if m[d-1] != 1:
      m[d-1] -= 1
      d += 1
    r[d-1] = f
    m[d-1] = int(sigma/f) + 1
    s = sigma % f
    if s != 0:
      d += 1
      r[d-1] = s
      m[d-1] = 1
    yield filter(lambda x: x > 0, r)
  
  
  
  
for x in next_partition(10):
  print x
  
  
