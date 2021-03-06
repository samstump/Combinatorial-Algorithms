import combalg
import time
import math
import unittest
import utility

def map_set_partition(a, cls):
  y = []
  for t in xrange(len(a)):
    x = reduce(lambda x,y: x+[a[y]] if cls[y] == t else x, range(len(a)), [])
    if x:
      y.append(x)
  return y

  
# TODO: the generator tests should test for uniqueness

class TestCombalgFunctions(unittest.TestCase):
  def test_powerset(self):
    n = 15
    i = 0
    a = range(n)
    for t in combalg.powerset(a):
      self.assertTrue(set(t) <= set(a))
      i += 1
    self.assertTrue(i == 2**n)

  def test_random_subset(self):
    n = 15
    a = range(n)
    trials = 100
    for i in xrange(trials):
      t = combalg.random_subset(a)
      self.assertTrue(set(t) <= set(a))
      
  def test_all_k_subsets(self):
    n = 15
    k = 6
    a = range(n)
    i = 0
    for t in combalg.all_k_subsets(a,k):
      self.assertTrue(len(t) == k)
      self.assertTrue(set(t) <= set(a))
      i += 1
    self.assertTrue(i == combalg.nCk(n,k))
    
  def test_random_k_subset(self):
    n = 15
    k = 6
    a = range(n)
    trials = 100
    for i in xrange(trials):
      t = combalg.random_k_subset(a,k)
      self.assertTrue(len(t) == k)
      self.assertTrue(set(t) <= set(a))
      
  def test_compositions(self):
    n = 15
    k = 6
    for t in combalg.compositions(n,k):
      self.assertTrue(len(t) == k)
      self.assertTrue(sum(t) == n)
  
  def test_random_compositions(self):
    n = 15
    k = 6
    trials = 100
    for i in xrange(trials):
      t = combalg.random_composition(n,k)
      self.assertTrue(len(t) == k)
      self.assertTrue(sum(t) == n)
      
  def test_permutations(self):
    n = 8
    a = range(n)
    for t in combalg.permutations(a):
      self.assertTrue(len(t) == n)
      self.assertTrue(sorted(t) == a)

  def test_random_permutation(self):
    n = 6
    trials = 5 * math.factorial(n)
    a = range(n)
    for i in xrange(trials):
      t = combalg.random_permutation(a)
      self.assertTrue(len(t) == n)
      self.assertTrue(sorted(t) == a)

  def test_integer_partitions(self):
    n = 15
    for t in combalg.integer_partitions(n):
      self.assertTrue(sum(t) == n)
  
  def test_random_integer_partition(self):
    n = 15
    trials = 100
    for i in xrange(trials):
      t = combalg.random_integer_partition(n)
      self.assertTrue(sum(t) == n)
      
  def test_set_partitions(self):
    a = range(6)
    n = len(a)
    for pop,cls,nc in combalg.set_partitions(n):
      y = map_set_partition(a, cls)
      # union of all partition elements == a
      self.assertTrue(set().union(*y) == set(a))
      # intersection of all partitions == {}
      self.assertTrue(set().intersection(*y) == set())

  def test_random_set_partitions(self):
    a = ['a','b','c','d','e','f']
    trials = 100
    for i in xrange(trials):
      # let p = the given set partition
      p = combalg.random_set_partition(a)
      # union of members of p == a
      self.assertTrue(set().union(*p) == set(a))
      # intersection of members of p == {}
      self.assertTrue(set().intersection(*p) == set())
      
  def test_random_rooted_tree(self):
    n = 10
    trials = 1000
    for i in xrange(trials):
      t = combalg.random_rooted_tree(n)
      edge_list = []
      for j in xrange(1,len(t)):
        edge_list.append((j,t[j]))
      self.assertTrue(len(edge_list) == n-1)
      self.assertTrue(utility.is_tree(n,edge_list))
#
#
#
# execute
suite = unittest.TestLoader().loadTestsFromTestCase(TestCombalgFunctions)
unittest.TextTestRunner(verbosity=2).run(suite)
