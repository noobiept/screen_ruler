#!/usr/bin/env python3
import sys
import json
import glob
import zipfile
import argparse
import subprocess

NAME = 'screen_ruler'
VERSION = '2.0.0'
BUILD_NAME = f'{NAME}-{VERSION}.zip'


def build():
    with zipfile.ZipFile(BUILD_NAME, 'w') as zip:
        for file in glob.glob('source/**/*.py', recursive=True):
            zip.write(file)

        zip.write('setup.py')


def getBuildName():
    sys.stdout.write(BUILD_NAME)


def checkFormat():
    exitCode = subprocess.call(
        ['autopep8', '--diff', '--recursive', '--exit-code', 'source/'])
    sys.exit(exitCode)


def runFormat():
    exitCode = subprocess.call(
        ['autopep8', '--recursive', '--in-place', 'source/'])
    sys.exit(exitCode)


if __name__ == '__main__':

    possibleArgs = {
        'build': build,
        'getBuildName': getBuildName,
        'checkFormat': checkFormat,
        'runFormat': runFormat
    }

    keys = possibleArgs.keys()
    keysStr = ', '.join(keys)

    parser = argparse.ArgumentParser()
    parser.add_argument('action', type=str,
                        help=f"Possible arguments: {keysStr}")

    args = parser.parse_args()
    action = args.action

    function = possibleArgs.get(action)

    if function:
        function()

    else:
        print(f"Wrong argument: '{action}' (needs to be one of: {keysStr})")
        sys.exit(1)
