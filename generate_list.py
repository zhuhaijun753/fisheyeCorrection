#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Tue May  9 17:39:15 2017

@author: andrew
"""

import os
import os.path as op
import sys


#    idir = './front_54'
idir = sys.argv[1]

image_list = os.listdir(idir)
image_list = [op.join(idir,x) for x in image_list if op.isfile(op.join(idir, x))]
                                                 
list_len = len(image_list)

for i in range(list_len -1):
    image_list[i] = image_list[i] + '\n'
    
with open('image_list.txt', 'w') as f:
    f.writelines(image_list)
        


