# Screen Ruler #


## Versions ##


There's two versions of the program, one written in c++ with gtkmm and other in python with Qt (PySide).

I initially wrote the one in c++, but since gtkmm3 doesn't support any other platform than linux (and doesn't look like it will), I ended up re-writing in Qt.


## Units ##


Just a quick heads up about different units. 

The ruler is accurate for pixels (since that's how its drawn), but for the remaining units, its not 100% accurate.

This happens since you sometimes need a line in between two pixels (which isn't possible, a pixel is the smallest unit possible). Therefore you'll see some lines not perfectly separated (since the proper position would be in between two pixels).



In the options window, there's a label that shows the distance from the origin of the ruler to where the mouse is. That's probably a better way to measure.


On windows7 (at least), the centimeter/inch isn't working correctly (wrong values). Works in linux though.



# Installing #

## Python/PySide ##

Dependencies:

* [Python3](http://python.org)
* [PySide](http://qt-project.org/wiki/PySide)


Just execute **python3 main.py** to run.

On windows, you can just create a shortcut to run. The target would be something like:

    C:\Python33\pythonw.exe C:\path\to\ruler\main.py


## c++/gtkmm ##

### Dependencies ###

You'll need to install automake and autoconf, to be able to compile the code.

This program also depends on gtkmm3, so you'll need both the library and the development files.


### Compiling ###

Download the code, open a console in that folder and write:

* **autoreconf --force --install**
* **./configure**
* **make**

It will be created an executable file (among other things) named screen_ruler, which you use to start the program. 



----


# Other #

## Images ##

![Ruler](https://bitbucket.org/drk4/screen_ruler/wiki/images/ruler.png)


## keyboard shortcuts ##

Ruler window:

* alt + ( o )ptions --> open the options window
* alt + ( r )otate  --> rotate the ruler window
	

Options/about window:

* esc key --> closes the window
	


