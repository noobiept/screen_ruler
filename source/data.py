import json
from pathlib import Path

from PySide6.QtGui import QColor

CONFIG_PATH = Path.home() / '.config' / 'screen_ruler' / 'config.json'


def isValidUnitValue(value):
    return value in ('px', 'cm', 'inch')


def isValidColorValue(value):
    return 0 <= value <= 255


def setIfValid(storage, options, key, type, validate=None):
    option = storage.get(key)

    if isinstance(option, type):
        if validate and not validate(option):
            return

        options[key] = option


def setColorIfValid(storage, options, key):
    color = storage.get(key)

    if isinstance(color, dict):
        if set(('red', 'green', 'blue', 'alpha')).issubset(color):
            red = color['red']
            green = color['green']
            blue = color['blue']
            alpha = color['alpha']

            if (
                not isValidColorValue(red) or
                not isValidColorValue(green) or
                not isValidColorValue(blue) or
                not isValidColorValue(alpha)
            ):
                return

            options[key] = QColor(red, green, blue, alpha)


class Data():
    @staticmethod
    def optionsToStorage(options, currentState):
        """
            Convert from the 'options' structure to the 'storage' data structure.
        """
        storage = options.copy()

        # need to get the individual rgba() from the QColor so that we can serialize it into json
        background = storage['background_color']
        lines = storage['lines_color']
        divisions = storage['divisions_color']

        storage['background_color'] = {
            'red': background.red(),
            'green': background.green(),
            'blue': background.blue(),
            'alpha': background.alpha()
        }
        storage['lines_color'] = {
            'red': lines.red(),
            'green': lines.green(),
            'blue': lines.blue(),
            'alpha': lines.alpha()
        }
        storage['divisions_color'] = {
            'red': divisions.red(),
            'green': divisions.green(),
            'blue': divisions.blue(),
            'alpha': divisions.alpha()
        }

        storage['ruler_width'] = currentState['width']
        storage['ruler_height'] = currentState['height']
        storage['options_opened'] = currentState['optionsOpened']

        return storage

    @staticmethod
    def storageToOptions(storage):
        """
            Convert from the 'storage' structure to the 'options' data structure.
        """
        options = {}

        setIfValid(storage, options, 'units', str, isValidUnitValue)
        setIfValid(storage, options, 'always_above', bool)
        setIfValid(storage, options, 'horizontal_orientation', bool)
        setIfValid(storage, options, 'ruler_width', int)
        setIfValid(storage, options, 'ruler_height', int)
        setIfValid(storage, options, 'options_opened', bool)
        setIfValid(storage, options, 'division_lines', bool)

        # deal with the colors (from rgba values to a QColor object)
        setColorIfValid(storage, options, 'background_color')
        setColorIfValid(storage, options, 'lines_color')
        setColorIfValid(storage, options, 'divisions_color')

        return options

    def __init__(self):
        self.options = {
            'units': 'px',  # px / cm / inch
            'always_above': False,
            'horizontal_orientation': True,
            'background_color': QColor(222, 212, 33, 210),
            'lines_color': QColor(0, 0, 0, 255),
            'divisions_color': QColor(0, 0, 255, 255),
            'ruler_width': 500,
            'ruler_height': 50,
            'options_opened': False,
            'division_lines': False,
        }
        self.load()

    def get(self, key):
        return self.options[key]

    def update(self, key, value):
        self.options[key] = value

    def load(self):
        """
            Load the options from the configuration file.
        """
        try:
            text = CONFIG_PATH.read_text(encoding='utf-8')
            storage = json.loads(text)

        except (FileNotFoundError, ValueError):
            return

        self.options.update(Data.storageToOptions(storage))

    def save(self, currentState):
        """
            Write the current options values to the configuration file.
        """
        storage = Data.optionsToStorage(self.options, currentState)

        # make sure the directory exists before trying to create the configuration file
        if not CONFIG_PATH.parent.exists():
            CONFIG_PATH.parent.mkdir(parents=True, exist_ok=True)

        CONFIG_PATH.write_text(json.dumps(storage), encoding='utf-8')
