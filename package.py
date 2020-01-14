#!/usr/bin/env python3
import os
import sys
import json
import glob
import zipfile
import argparse
import subprocess


NAME = 'screen_ruler'
VERSION = '2.2.0'
OUTPUT_PATH = f'build/{NAME}-{VERSION}.zip'


def build():
    os.makedirs(os.path.dirname(OUTPUT_PATH), exist_ok=True)

    with zipfile.ZipFile(OUTPUT_PATH, 'w') as zip:
        for file in glob.glob('source/**/*.py', recursive=True):
            zip.write(file)

        zip.write('setup.py')
        zip.write('package.py')


def createExecutable():
    exitCode = subprocess.call(
        ['pyinstaller', '--name=Screen Ruler', '--windowed', 'source/go.py'])
    sys.exit(exitCode)


def getOutputPath():
    sys.stdout.write(OUTPUT_PATH)


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
        'createExecutable': createExecutable,
        'getOutputPath': getOutputPath,
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
