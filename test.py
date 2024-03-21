# -*- coding: utf-8 -*-
import scipy.stats as st
import math
import random

wilcoxon_table = {5e-2: {6: 0, 7: 2, 8: 4, 9: 6, 10: 8, 11: 11, 12: 14},
                  1e-2: {7: 0, 8: 2, 9: 3}}

def wilcoxon_test(a, b, alpha = .05):

N = len(a) # Number of datasets
    # Compute the differences and keep the signs
    differences, signs = ([abs(a[i] - b[i]) for i in xrange(N)],
                          [a[i] - b[i] for i in xrange(N)])
    tmp = sorted(differences)
    # The rank is the median between the index of the first element equal
    # to v in tmp (index(v)+1) and the index of the last element equal to v
    # (index(v)+count(v))
    ranks = [(tmp.count(v)+tmp.index(v)*2+0x1)/2e0 for v in differences]