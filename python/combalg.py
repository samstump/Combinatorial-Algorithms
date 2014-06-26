# combalg.py
'''
  This package is a collection of combinatorial algorithms primarily for enumeration and random selection
  of combinatorial objects like sets, subsets, permutationsm partitions and compositions.  The source(s) for
  these algorithms comes from
    [1] "Combinatorial Algorithms (for Computers and Calculators), 2nd ed.", Nijenhuis & Wilf, Academic Press, 1978
        http://www.math.upenn.edu/~wilf/website/CombinatorialAlgorithms.pdf
    [2] "Fast Algorithms for Generating Integer Partitions", Zoghbi & Stojmenovic, 1998
        http://www.site.uottawa.ca/~ivan/F49-int-part.pdf
'''

import random

'''
  A generator that produces all subsets of the input 'set' of elements.  The input doesn't need 
  to be a set, any iterable will do (I think).  The point is that the elements need not be unique,
  but will be treated as unique with respect to the subset generation.  
  For example:
    powerset(['a','a']) will give the sequence ['a','a'], ['a'], ['a'], [] 
'''
def powerset(elements):
  if len(elements) == 0:
    yield []
  else:
    for item in powerset(elements[1:]):
      yield [elements[0]]+item
      yield item
'''
  A generator that produces all k-element subsets of the input 'set' of elements in lexicographic order.
  The algorithm essentially goes as follows:  
    - find the position closest to the far end of the list that can be incremented and increment it
    - set all following positions as increasing sequence.
    - done when k=0 or n (only a single subset in these cases), or when a[0] = n-k
  Probably needs a more formal explanation.
'''
def all_k_subsets(elements, k):
  n = len(elements)
  a = range(k)
  h = 0
  done = False
  first = True
  while not done:
    if not first:
      h = 0 if a[k-h-1] < n-h-1 else h+1
      a[k-h-1] += 1
      for j in xrange(k-h,k):
        a[j] = a[j-1] + 1
    done = (k == 0) or (k == n) or (a[0] == n - k)
    first = False
    yield map(lambda x: elements[x], a)
    
'''
  Returns a random subset of a set of elements.  Equivalent to randomly selecting an element of
  powerset(elements).  For each element of the universal set, select it to be in the subset with p=0.5
'''
def random_subset(elements):
  return filter(lambda x: random.random() <= 0.5, elements)
  
'''
  Returns a random k-element subset of a set of elements.  This is a clever algorithm, explained in [1]
  but not actually implemented.  Essentially starting with the first element of the universal set, include
  it in the subset with p=k/n.  If the first was selected, select the next with p=(k-1)/(n-1), if not
  p = k/(n-1), etc.  The 'pythonic' implementation included in the comments is roughly half as performant
  as the iterative solution given in the source.

  def random_k_subset(elements, k):
  return reduce(lambda (a,c1,c2),y: (a+[y],c1-1,c2-1) if random.random() <= float(c1)/c2 else (a,c1,c2-1), elements, ([],k,len(elements)))[0]
'''
def random_k_subset(elements, k):
  a = []
  c1 = k
  c2 = len(elements)
  i = 0
  while c1 > 0:
    if random.random() <= float(c1)/c2:
      a.append(elements[i])
      c1 -= 1
    c2 -= 1
    i += 1
  return a

'''
  A generator that returns all of the compositions of n into k parts, i.e., k terms that sum to n, 
  including zero terms, and order is important.  For example, the compositions of 2 into 2 parts:
  compositions(2,2) = [2,0],[1,1],[0,2].
  NOTE: There are C(n+k-1,n) partitions of n into k parts.
'''
def compositions(n, k):
  t = n
  h = 0
  a = [0]*k
  a[0] = n
  yield a
  while a[k-1] != n:
    if t != 1:
      h = 0
    t = a[h]
    a[h] = 0
    a[0] = t-1
    a[h+1] += 1
    h += 1
    yield a
'''
  Returns a random composition of n into k parts
'''
def random_composition(n,k):
  a = random_k_subset(range(1,n+k), k-1)
  r = [0]*k
  r[0] = a[0] - 1
  for j in range(1,k-1):
    r[j] = a[j] - a [j-1] - 1
  r[k-1] = n + k - 1 - a[k-2]
  return r

'''
   A generator that returns all permutations of the input list
'''
def permutations(a):
  if len(a) <= 1:
    yield a
  else:
    for perm in permutations(a[1:]):
      for i in xrange(len(a)):
        yield perm[:i] + a[0:1] + perm[i:]
'''
  Returns a random permutation of the input list
'''
def random_permutation(elements): 
  a = list(elements)
  n = len(a)
  for m in xrange(n-1):
    l = m + int(random.random() * (n - m))
    tmp = a[l]
    a[l] = a[m]
    a[m] = tmp
  return a
  
'''
  A generator that returns all integer partitions of n, that is all x0,x1,x2,...,xk such that sum(xi)k = n and xi > 0
  Algorithm ZS1 in [2]
'''  
def integer_partitions(n):
  x = [1]*n
  x[0] = n
  m = 1
  h = 1
  yield x[:m]
 
  while x[0] != 1:
    if x[h-1] == 2:
      m += 1
      x[h-1] = 1
      h -= 1
    else:
      r = x[h-1] - 1
      t = m - h + 1
      x[h-1] = r
      while t >= r:
        x[h] = r
        t = t - r
        h += 1
      if t == 0:
        m = h
      else:
        m = h + 1
      if t > 1:
        x[h] = t
        h += 1
    yield x[:m]

'''
  Returns a random integer partition of n
  TODO: 
    the local function 'num_partitions(n)' is not smart at all
    the local functions 'num_partitions(n)' and 'choose_dj(n, rho)' can be combined, as in [1].  I have had better
      luck implementing algorithms from [1] using the mathematical description and NOT using the fortran source.
'''
def random_integer_partition(n):
  P = {}
  def num_partitions(n):
    if n <= 1:
      return 1
    if n not in P:
      sum = 1
      for j in xrange(1,n):
        d = 1
        while n - j*d >= 0:
          sum += d * num_partitions(n - j*d)
          d += 1
      P[n] = sum
    else:
      sum = P[n]
    return sum/n

  def choose_dj(n, rho):
    denom = num_partitions(n)
    if n <= 1:
      return 1,1
    sum = 0
    for j in xrange(1,n):
      d = 1
      while n - j*d >= 0:
        sum += float(d * num_partitions(n - j*d))/n
        if float(sum)/denom >= rho:
          return d,j      
        d += 1
    return 1,n

  a = []
  m = n
  while m > 0:
    d,j = choose_dj(m, random.random())
    a = a + [d]*j
    m -= j*d
  return sorted(a,reverse=True)

'''
  A generator that returns all set partitions of [n] = {0,1,2,...,n-1}.  The result is returned as a
  3-tuple (p,q,nc):
    p - a list where p[i] is the population of the i-th equivalence class
    q - a list where q[i] is the equivalence class of i
    nc - the number of equivalence classes in the partition
'''
def set_partitions(n):
  p = [n] + [0]*(n-1)
  nc = 1
  q = [0]*n
  m = n
  yield p,q,nc
  
  while nc != n:
    m = n
    while True:
      l = q[m-1]
      if p[l] != 1:
        break
      q[m-1] = 0
      m -= 1
    z = m - n
    if z < 0:
      for i in xrange(0,z,-1):
        p[nc+i-1] = 0
    nc += z
    p[0] -= z
    if l == nc - 1:
      p[nc] = 0
      nc += 1
    q[m-1] = l+1
    p[l] -= 1
    p[l+1] += 1
    yield p,q,nc
