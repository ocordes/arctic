#! /usr/bin/env python

# written by: Oliver Cordes 2017-04-03
# changed by: Oliver Cordes 2017-04-03

# System imports

try:
    from setuptools import setup
    from  setuptools.extension import Extension
except:
    from distutils.core import *
    from distutils      import sysconfig


import sys, os


# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()


include_dirs = [ numpy_include, '../src' ]


if sys.platform == 'darwin':
    include_dirs.append( '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/' )
    os.environ['LDFLAGS'] = '-stdlib=libc++'
    if 'CC' not in os.environ.keys():
        os.environ['CC'] = 'clang++'
    if 'CXX' not in os.environ.keys():
        os.environ['CXX'] = 'clang++'
else:
    if 'CC' not in os.environ.keys():
        os.environ['CC'] = 'g++'
    if 'CXX' not in os.environ.keys():
        os.environ['CXX'] = 'g++'


# view extension module
_arctic = Extension("_arctic",
                   ["arctic.i", "test_valarray.cc"],
                   include_dirs = include_dirs,
                   library_dirs = ['../build'],
                   libraries = [ 'arctic' ],
                   swig_opts=['-c++'],
                   extra_compile_args = [ '-std=c++11' ]
                   )

# NumyTypemapTests setup
setup(  name        = "artic python module",
        description = "This module is a python wrapper around the artic program!",
        author      = "Oliver Cordes",
        version     = "1.0",
        ext_modules = [_arctic]
        )
