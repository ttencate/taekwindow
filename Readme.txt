Taekwindow
==========

Taekwindow is a simple, lightweight and free program that allows you to
manipulate floating windows in Microsoft Windows in a way similar to
many X11 window managers.

Installation
------------

Simply extract the archive to any place you like. Make sure that the
files taekwindow.exe and taekwindowhooks.dll are in the same directory.

If you want to load the program when you log on to Windows, add a
shortcut to taekwindow.exe in the Startup folder in your Start Menu.

Using the program
-----------------

Start taekwindow.exe to load the program. Nothing will seem to happen,
but you can now do the following:
- Hold Alt and drag a window with the left mouse button to move it.
- Hold Alt and drag a window with the right mouse button to resize it.

To stop the program, just run taekwindow.exe again. It will not load
itself, but rather stop the running instance and then quit. You can also
stop it through the Task Manager if you prefer.

Known bugs
----------

- When resizing cmd.exe and releasing the button outside the window,
  you will still be resizing it even though you're no longer holding the
  button.
- PuTTY seems to resize but snaps back to its old size as soon as the
  button is released.
- MDI interfaces do not always work properly. In Photoshop all is well,
  in Opera the main window is moved instead.

Planned features
----------------

- A tray icon to view and control whether the program is running.
- A configuration program to set which mouse buttons and modifier keys
  to use.
- Windows snapping to screen edges and to each other while moving and
  resizing.
- Copying by selecting text, pasting with the middle mouse button, just
  as in X.
- Raising and lowering windows with Alt+middle mouse button.
- Scrolling the window under the cursor with the scroll wheel, instead
  of the window that currently has focus (which is Windows' normal,
  annoying behaviour).
- Sloppy focus (focus-follows-mouse) with optional window raising.

Contact
-------

TODO

Legal
-----

TODO
