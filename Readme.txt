Taekwindow
==========

Taekwindow is a simple, lightweight and free program that allows you to
manipulate floating windows in Microsoft Windows in a way similar to
many X11 window managers.

Installation
------------

1. You need to have the Microsoft .NET Framework, version 2.0 or later,
   installed to use the configuration tool. You can get the latest version
   of the framework here:
   http://msdn2.microsoft.com/en-us/netframework/aa569263.aspx

2. To install Taekwindow, simply extract the archive to any place you like.
   Make sure that the files are all in the same directory.

3. Run taekwindowconfig.exe to configure the Taekwindow. The configuration
   tool can create a shortcut in the Startup section of your Start Menu, so
   Taekwindow is started when you log on to your computer.

4. Optional: create a shortcut to taekwindowconfig.exe in your Start Menu.

Using the program
-----------------

Start taekwindow.exe to load the program. Nothing will seem to happen,
but you can now do the following:
- Hold left Alt and drag a window with the left mouse button to move it.
- Hold left Alt and drag a window with the right mouse button to resize it.
(These buttons are configurable through the configuration tool.)

To stop the program, just run taekwindow.exe again. It will not load
itself, but rather stop the running instance and then quit. You can also
stop it through the Task Manager if you prefer.

Configuring the program
-----------------------

You can configure the program using the supplied configuration tool,
taekwindowconfig.exe. Its usage should be self-explanatory.

If the tool refuses to start, make sure that the .NET framework is
installed. See Installation, above, for details.

Known bugs
----------

Please see the Bugs Tracker at:
http://sourceforge.net/tracker/?group_id=185457&atid=913353

You can view the list of bugs in the current release as well as submit
any new bugs you've discovered.

Planned features
----------------

Please see the Feature Requests Tracker at:
http://sourceforge.net/tracker/?group_id=185457&atid=913356

Apart from a list of planned features, this also contains all feature
requests submitted by users. You can of course submit your own.

Version history
---------------

0.2.1:
- FIX: moving and resizing now works properly when dragging title bar or borders

0.2:
- added a configuration tool as per
  http://sourceforge.net/tracker/index.php?func=detail&aid=1629762&group_id=185457&atid=913356
- made the modifier and mouse buttons, as well as closest-corner resizing,
  into options as per
  http://sourceforge.net/tracker/index.php?func=detail&aid=1653698&group_id=185457&atid=913356
- added an option to the configuration tool for creating/deleting a Startup
  shortcut in the Start Menu

0.1.1:
- implemented closest-corner resizing as per
  http://sourceforge.net/tracker/index.php?func=detail&aid=1632182&group_id=185457&atid=913356

0.1:
- very first version
- native Win32 executable facilitating moving and resizing with Alt

Contact
-------

Taekwindow is written by Thomas ten Cate. The preferred way to contact
the author is through the Taekwindow web page, located at:
http://taekwindow.sf.net/

Contributors
------------

The following people have contributed to the current version of
Taekwindow:
- Eamon Nerbonne and Mark IJbema have tested early versions of the
  program and provided valuable comments and suggestions.
- Sylvain Vignaud provided a patch to blacklist certain applications
  that require Alt-dragging in their normal mode of operation.

Legal (modified BSD-style license)
----------------------------------

Copyright (c) 2007, Thomas ten Cate
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
