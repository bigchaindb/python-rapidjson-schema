#!/usr/bin/env python

import os
import sys
from distutils.core import setup, Extension
from distutils import sysconfig


if sys.version_info < (3,):
    raise NotImplementedError("Only Python 3+ is supported.")


ROOT_PATH = os.path.abspath(os.path.dirname(__file__))

if not os.path.isdir(os.path.join(ROOT_PATH, 'rapidjson', 'include')):
    raise RuntimeError("RapidJSON sources not found: if you cloned the git repository,"
                       " you should initialize the rapidjson submodule as explained in"
                       " the README.rst; in all other cases you may want to report the"
                       " issue.")

extension_options = {
    'sources': ['schema.cpp'],
    'include_dirs': ['./rapidjson/include'],
}

cc = sysconfig.get_config_var('CC')
if cc and 'gcc' in cc:
    cflags = sysconfig.get_config_var('CFLAGS')
    # Avoid warning about invalid flag for C++
    if cflags and '-Wstrict-prototypes' in cflags:
        cflags = cflags.replace('-Wstrict-prototypes', '')
        sysconfig.get_config_vars()['CFLAGS'] = cflags

    # Add -pedantic, so we get a warning when using non-standard features, and
    # -Wno-long-long to pacify old gcc (or Apple's hybrids) that treat "long
    # long" as an error under C++ (see issue #69)
    extension_options['extra_compile_args'] = ['-pedantic', '-Wno-long-long']


schema = Extension('rapidjson_schema', **extension_options)


setup(
    name='python-rapidjson-schema',
    version='0.1.1',
    license='Apache2',
    description='RapidJSON schema validation',
    author='BigchainDB Developers',
    author_email='dev@bigchaindb.com',
    url='https://github.com/bigchaindb/python-rapidjson-schema/',
    ext_modules=[schema],
    python_requires='>=3.4',
)
