Qt Charts 5e
============

Qt Charts module provides a set of easy to use chart components. It uses
the Qt Graphics View Framework, therefore charts can be easily integrated
to modern user interfaces.

### Enhanced
This is an enhanced version of QtCharts5 module. I have tried to keep it very close to
the original module. Following changes I have done:
	- Now we can set the marker size for QLineSeries when pointsVisible(...) is set to true
	- A missing feature was impulses/vertical lines from a data point. It's implemented in QSticksSeries

System Requirements
-------------------

- Qt 5.4 or newer
- For QML applications QtQuick 2 is required
- Manipulating Qt Charts graphs with QML Designer requires Qt Creator 3.3 or newer

Building
--------

Configure the project with qmake:

    `qmake`

After running qmake, build the project with make:

    - (Linux) `make`
    - (Windows with MinGw) `mingw32-make`
    - (Windows with Visual Studio) `nmake`
    - (OS X) `make`

The above generates the default makefiles for your configuration, which is typically
the release build if you are using precompiled binary Qt distribution. To build both
debug and release, or one specifically, use one of the following qmake lines instead.

For debug builds:

    `qmake CONFIG+=debug`
    `make`

  or

    `qmake CONFIG+=debug_and_release`
    `make debug`

For release builds:

    `qmake CONFIG+=release`
    `make`

  or

    `qmake CONFIG+=debug_and_release`
    `make release`

For both builds (Windows/OS X only):

    `qmake CONFIG+="debug_and_release build_all"`
    `make`

After building, install the module to your Qt directory:

    `make install`

If you want to uninstall the module:

    `make uninstall`

Building as a statically linked library
---------------------------------------

The same as above applies, you will just have to add static to the CONFIG:

    `qmake CONFIG+=static`

Documentation
-------------

The documentation can be generated with:

    `make docs`

The documentation is generated into the doc folder under the build folder.
Both Qt Assistant (qtcharts.qch) and in HTML format (qtcharts subfolder)
documentation is generated.

Please refer to the generated documentation for more information:

    `doc/qtcharts/qtcharts-index.html`

Known issues
------------

- Zooming too far or too close may eventually freeze the zooming and scrolling.
