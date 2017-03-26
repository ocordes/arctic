#!/usr/bin/env python

import arctic
import numpy as np

# main
a = np.array( [0,1,2], dtype='int32'  )

arctic.test_function( a )

print( a )
