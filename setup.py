from setuptools import setup, find_packages

setup(
    name="screen_ruler",
    version="2.0.0",
    description="Screen ruler application",
    packages=['source', 'source.ruler'],
    entry_points={
        "console_scripts": ["screen_ruler=source.go:go"]
    }
)
