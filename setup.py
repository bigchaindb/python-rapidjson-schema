#!/usr/bin/env python

from distutils.core import setup, Extension

schema = Extension('rapidjson_schema', sources=['schema.cpp'])

setup(
    name='python-rapidjson-schema',
    version='0.1',
    description='RapidJSON schema validation',
    author_email='dev@bigchaindb.com',
    url='https://www.python.org/sigs/distutils-sig/',
    ext_modules=[schema],
)
