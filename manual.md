---
layout: page
title: Manual
---

User's Manual
=============

System requirements
-------------------

Taekwindow works on Windows XP and above (that is, Vista and 7), both 32-bits and 64-bits versions.
It is tested on 32-bits Windows XP and 64-bits Windows 7.

Windows 2000 is no longer officially supported, but might still work.

Installation
------------

1. To install Taekwindow, simply extract the archive to any place you like. You need only the `taekwindow.exe` file to use the program.
2. Through the Preferences dialog, you can create a shortcut in the Startup section of your Start Menu, so Taekwindow is started when you log on to your computer.

Usage
-----

Start `taekwindow.exe` to load the program. A new icon will appear in the taskbar notification area (next to the clock, also known as the system tray). You can now do the following:

* Hold left Alt and drag a window with the left mouse button to move it. If you have multiple monitors, you can also move maximized windows between monitors in this way.
* Hold left Alt and drag a window with the right mouse button to resize it. If you resize a maximized window, it will automatically be unmaximized.
* Middle-click on the title bar of a window to push it to the background.
* Use the scroll wheel on the window under the mouse cursor, instead of the window that happens to have the focus.

The icon in the taskbar can be used in the following ways:

* Left-click the icon to completely disable all features of Taekwindow. Use this in case some application needs to handle the events that Taekwindow is catching, but you don't want to quit Taekwindow completely. The icon will turn gray to indicate that it is disabled; left-click again to re-enable.
* Double-click the icon to open the Preferences dialog.
* Right-click the icon to show a menu, through which you can enable/disable the program, open the Preferences dialog, and exit Taekwindow completely.

You can exit the program via the taskbar icon. If the icon is invisible, you can either use the button in the Preferences dialog to exit, or simply end the process through Windows Task Manager.

Configuration
-------------

You can configure the program by right-clicking the notification area icon and choosing Preferences. This will open the Preferences dialog; its usage should be self-explanatory.

If you chose to hide the taskbar icon, you can access the Preferences dialog by running `taekwindow.exe` a second time.

Version history
---------------

### 0.3.1

* settings can now optionally be stored in an INI file for portability (see [tracker item](http://sourceforge.net/tracker/?func=detail&aid=2926612&group_id=185457&atid=913353))
* Bugfix: the menu no longer gets activated because of pressing Alt (see [tracker item](http://sourceforge.net/tracker/?func=detail&aid=2920245&group_id=185457&atid=913353))

### 0.3

* the .NET platform is no longer required (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2920175&group_id=185457))
* on a 64-bits platform, moving and resizing now works for both 32-bits and 64-bits applications (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2809001&group_id=185457))
* Bugfix: scrolling now works correctly on monitors to the left or top of the primary monitor (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2314897&group_id=185457))
* Bugfix: pushing windows to the back with the middle mouse button can now be disabled (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=1999554&group_id=185457))
* Bugfix: the tray icon is now shown even if no settings are present in the registry (see* [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2145527&group_id=185457))
* Bugfix: moving an unfocused window behind a focused window no longer causes the focused window to move (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2210322&group_id=185457))
* Bugfix: the iTunes window can now be moved and resized (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2577045&group_id=185457))
* Bugfix: horizontal scroll wheels are now also sent to the right window (see [tracker item](http://sourceforge.net/tracker2/?func=detail&atid=913353&aid=2587696&group_id=185457))

### 0.2.4

* Bugfix: Google Talk now responds to move and resize actions (see [tracker item](http://sourceforge.net/tracker2/?func=detail&aid=2120039&group_id=185457&atid=913353))
* Bugfix: Google Chrome now responds to move and resize actions (see [tracker item](http://sourceforge.net/tracker2/?func=detail&aid=2095157&group_id=185457&atid=913353))
* Bugfix: the scroll wheel now works correctly in mIRC when it has the focus (see [tracker item](http://sourceforge.net/tracker2/?func=detail&aid=2000712&group_id=185457&atid=913353))
* Bugfix: Microsoft Word 2007 now moves and resizes correctly (see [tracker item](http://sourceforge.net/tracker2/?func=detail&aid=2000149&group_id=185457&atid=913353))

### 0.2.3

* windows are no longer raised when they are moved or resized (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1777612&group_id=185457&atid=913353))
* windows are no longer activated when they are moved or resized (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1777609&group_id=185457&atid=913353))
* the scroll wheel optionally affects the window under the mouse cursor, instead of the focused window (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1629767&group_id=185457&atid=913353))
* maximized windows can now be moved across multiple monitors while remaining maximized (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1939201&group_id=185457&atid=913353))
* maximized windows can now be resized and will become unmaximized
* a code overhaul should make it much easier to implement future improvements
* Bugfix: the error message "Specified cast is not valid" no longer appears when starting the configuration tool after an upgrade from 0.2.1 or earlier (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1908683&group_id=185457&atid=913353))
* Bugfix: pushing back an inactive window no longer raises the active one (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1905700&group_id=185457&atid=913353))
* Bugfix: Alt no longer "hangs" after returning to a desktop that was locked with Ctrl+Alt+Del (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1982738&group_id=185457&atid=913353))

### 0.2.2

* added an optional system tray icon (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1629764&group_id=185457&atid=913353))
* the mouse cursor changes while moving or resizing
* middle mouse click on the title bar pushes a window to the background
* Bugfix: when no settings are present in the registry, the configuration tool is now filled with default values instead of none at all
* Bugfix: it is no longer possible to start multiple instances of the configuration tool

### 0.2.1

* Bugfix: moving and resizing now works properly when dragging title bar or borders (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1653063&group_id=185457&atid=913353))
* Bugfix: moving and resizing now works on MDI child windows, instead of the parent (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1629393&group_id=185457&atid=913353))
* Bugfix: Taekwindow should no longer occasionally behave as if Alt is pressed when it is not (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1894902&group_id=185457&atid=913353))

### 0.2

* added a configuration tool (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1629762&group_id=185457&atid=913353))
* made the modifier and mouse buttons, as well as closest-corner resizing, into options (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1653698&group_id=185457&atid=913353))
* added an option to the configuration tool for creating/deleting a Startup shortcut in the Start Menu
* Bugfix: the error message "This application has failed to start because the application configuration is incorrect. Reinstalling the application may fix this problem." no longer appears (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1682619&group_id=185457&atid=913353))

### 0.1.1

* implemented closest-corner resizing (see [tracker item](http://sourceforge.net/tracker/index.php?func=detail&aid=1632182&group_id=185457&atid=913353))

### 0.1

* moving with Alt+left button, resizing from the bottom right corner with Alt+right button

Thanks
------

I would like to thank the following people for their contributions to this project:

<dl>
	<dt>Eamon Nerbonne and Mark IJbema</dt><dd>tested early versions of the program and provided valuable comments and suggestions.</dd>
    <dt>Sylvain Vignaud</dt><dd>provided a patch to blacklist certain applications that require Alt-dragging in their normal mode of operation. I'm still planning to convert this into a configurable blacklist some day!</dd>
</dl>

License
-------

Copyright (c) 2006-2011, Thomas ten Cate.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
