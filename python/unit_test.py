import combalg
import time

# power set
a = [1,2,3]
print 'power set',a,':'
for t in combalg.powerset(a):
  print t
print

# k subsets  
b = [1,2,3,4,5]
k = 3
print k,'subsets of',b,':'
for t in combalg.all_k_subsets(b,k):
  print t
print

# random subsets
n = 10
print n,'random subsets of',b
for i in xrange(n):
  print combalg.random_subset(b)
print

# random k subset
c = range(10)
print n,'random',k,'subsets of [',c[0],'...',c[len(c)-1],']'
for i in xrange(n):
  print combalg.random_k_subset(c,k)
print

# compositions
print '2 compositions of 6'
for t in combalg.compositions(6,2):
  print t
print

# random compositions
print 'random compositions of 10 into 4 parts'
for t in xrange(10):
  print combalg.random_composition(10,4)
  
#TODO: enumerate permutations
#TODO: random permutations
#TODO: enumerate integer partitions
#TODO: random integer partitions

  