from distutils.core import setup
setup(name='donspendfrom',
      version='1.0',
      description='Command-line utility for donationcoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@donationcoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
