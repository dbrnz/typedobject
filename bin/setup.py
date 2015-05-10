from setuptools import setup
import py2exe

options = {
    'bundle_files': 1,
    'ascii': 1, # to make a smaller executable, don't include the encodings
    'compressed': 1, # compress the library archive
    }

setup(
    name = 'generate_tobj',
    console = [{'script': 'generate.py', 'dest_base': 'generate_tobj'}],
    options = {'py2exe': options},
    zipfile = None, # append zip-archive to the executable.
    )
