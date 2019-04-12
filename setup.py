from setuptools import setup, find_packages
from package import NAME, VERSION

setup(
    name=NAME,
    version=VERSION,
    description="Screen ruler application",
    packages=['source', 'source.ruler'],
    entry_points={
        "gui_scripts": ["screen_ruler=source.go:go"]
    }
)
