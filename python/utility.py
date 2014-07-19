# utility.py

'''
  takes an iterable and makes a sorted list of it and all iterable members, makes for nice display
'''
def listify(a):
  if a and hasattr(a, '__iter__'): 
    return sorted([listify(list(a)[0])] + listify(list(a)[1:]))
  else:
    return a
