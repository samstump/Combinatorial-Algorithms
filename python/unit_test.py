import combalg
import time
import math
import unittest

def nCr(n,r):
  return math.factorial(n)/math.factorial(r)/math.factorial(n-r)

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
    self.assertTrue(i == nCr(n,k))
    
  def test_random_k_subset(self):
    n = 15
    k = 6
    a = range(n)
    trials = 100
    for i in xrange(trials):
      t = combalg.random_k_subset(a,k)
      self.assertTrue(len(t) == k)
      self.assertTrue(set(t) <= set(a))
  
  # TODO: compositions
  # TODO: permutations
  # TODO: partitions

# execute
suite = unittest.TestLoader().loadTestsFromTestCase(TestCombalgFunctions)
unittest.TextTestRunner(verbosity=3).run(suite)