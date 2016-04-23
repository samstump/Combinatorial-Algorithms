# bloom_filter.py

import math
import hashlib
import random
import string
#from BitVector import BitVector

def hash_function(n, msg, m):
    h = hashlib.md5()
    h.update('{:03d}:{}'.format(n, msg))
    return long(h.hexdigest(), 16) % m

class bloom_filter:
    def __init__(self, m, k):
        self.m = m
        self.k = k
        self.bitvector = [False] * self.m
        
    def insert(self, value):
        for i in xrange(0, self.k):
            self.bitvector[hash_function(i, value, self.m)] = True
            
    def contains(self, value):
        present = True
        for i in xrange(0, self.k):
            if not self.bitvector[hash_function(i, value, self.m)]:
                present = False
                break;
        return present


def random_string(min_len, max_len):
    return ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for _ in range(random.randint(min_len, max_len)))

print 'reading word list...'
s = set()
with open('../data/enable.lexicon') as f:
    for line in f:
        line = line.strip()
        s.add(line)
        
pp = 1.0e-4
n = len(s)
factor = -math.log(pp) / (math.log(2) **2)
m = int(0.5 + factor * n)

# below is calculation for optimal number of hash functions
k = int(0.5 + float(m)/n * math.log(2))

# theoretical probability of a false positive
p = math.exp(-float(m)/n * math.log(2) ** 2)

print 'done!'
print 'n = {}'.format(n)
print 'factor = {:.3f}'.format(factor)
print 'm = {}'.format(m)
print 'k = {}'.format(k)
print 'p = {:.3g}% ({:.5e})'.format(100.0 * p, p)


bf = bloom_filter(m, k)

for q in s:
    bf.insert(q)

num_random_strings = 100000
fp_list = []
fn_list = []

for i in xrange(num_random_strings):
    x = random_string(4,12)
    in_bloom = bf.contains(x)
    in_set = x in s   
    if in_bloom and not in_set:
        fp_list.append(x)
    if not in_bloom and in_set:
        fn_list.append(x)

pfp = float(len(fp_list)) / num_random_strings
pfn = float(len(fn_list)) / num_random_strings

print "False Positives: {}".format(len(fp_list)), fp_list
print "Expected: {:.3f}".format(p * num_random_strings)
print 'Rate: {:.3g}% ({:.5e})'.format(100*pfp, pfp)

print "False Negatives: {}".format(len(fn_list)), fn_list
print "Expected: {:.3f}".format(0.00 * num_random_strings)
print 'Rate: {:.3g}% ({:.5e})'.format(100*pfn, pfn)

