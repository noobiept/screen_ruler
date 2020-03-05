import json
from pathlib import Path

from PySide2.QtGui import QColor

CONFIG_PATH = Path.home() / '.config' / 'screen_ruler' / 'config.json'


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
        options = storage.copy()

        # deal with the colors (from rgba values to a QColor object)
        backgroundColor = options['background_color']
        linesColor = options['lines_color']
        divisionsColor = options['divisions_color']

        if isinstance(backgroundColor, dict):
            options['background_color'] = QColor(
                backgroundColor['red'], backgroundColor['green'],
                backgroundColor['blue'], backgroundColor['alpha'])

        if isinstance(linesColor, dict):
            options['lines_color'] = QColor(
                linesColor['red'], linesColor['green'],
                linesColor['blue'], linesColor['alpha'])

        if isinstance(divisionsColor, dict):
            options['divisions_color'] = QColor(
                divisionsColor['red'], divisionsColor['green'],
                divisionsColor['blue'], divisionsColor['alpha'])

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

        self.options = Data.storageToOptions(storage)

    def save(self, currentState):
        """
            Write the current options values to the configuration file.
        """
        storage = Data.optionsToStorage(self.options, currentState)

        # make sure the directory exists before trying to create the configuration file
        if not CONFIG_PATH.parent.exists():
            CONFIG_PATH.parent.mkdir(parents=True, exist_ok=True)

        CONFIG_PATH.write_text(json.dumps(storage), encoding='utf-8')
