# Screen Ruler

![Image](images/ruler.png)

## Units

The ruler is accurate for pixels (since that's how its drawn), but for the remaining units, its not 100% accurate.

This happens since you sometimes need a line in between two pixels (which isn't possible, a pixel is the smallest unit possible). Therefore you'll see some lines not perfectly separated (since the proper position would be in between two pixels).

In the options window, there's a label that shows the distance from the origin of the ruler to where the mouse is. That's probably a better way to measure.

# Installation

-   `brew tap drk4/homebrew-tap https://github.com/noobiept/homebrew-tap/`
-   `brew install screen_ruler`

# Development

## Dependencies

-   [Python3](http://python.org)
-   [PySide2](https://wiki.qt.io/Qt_for_Python)

*   `brew install python`
*   `pip3 install -r requirements.txt`

## Run

-   With python.
    -   `python3 source/go.py`
-   Alternatively you can create an executable and run that.
    -   `python3 package.py createExecutable`
    -   The output is located at: `dist/Screen Ruler.app`

There's also a helper script (`package.py`) to build, check the formatting, etc.

`python3 package.py --help`

# Keyboard shortcuts

Ruler window:

-   alt + ( o )ptions --> open the options window
-   alt + ( r )otate --> rotate the ruler window

Options/about window:

-   esc key --> closes the window
