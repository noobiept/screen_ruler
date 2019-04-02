#!/usr/bin/env python3
import json
import glob
import zipfile


def build():
    with open('package.json', 'r') as f:
        package = json.load(f)

    name = package['name']
    version = package['version']

    with zipfile.ZipFile(f'{name}-{version}.zip', 'w') as zip:
        for file in glob.glob('source/*.py'):
            zip.write(file)

        zip.write('setup.py')


if __name__ == '__main__':
    build()
