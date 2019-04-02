from setuptools import setup

setup(
    name="screen_ruler",
    version="2.0.0",
    description="Screen ruler application"
    entry_points={
        "console_scripts": ["screen_ruler=source.main:run"]
    }
)
