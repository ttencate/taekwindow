---
layout: base
title: FAQ
---

Frequently Asked Questions
==========================

General
-------

### Why is it called Taekwindow?

Taekwindow is a contraction of _taekwondo_ (<a href="http://www.wtf.org/">WTF?</a>) and _window_. You are free to interpret this in any way you like, but the bottom line is that I just happen to like bad puns.

Common misspellings are "TaekWindow", "Taekwindows" and "Teakwindow".

The red and blue circle in the logo is called the _Taeguk_. It is the national symbol of Korea, best known from the Korean national flag. The blue part symbolizes the water around the land. Combined with the mouse cursor, whose origin is obvious, I think it makes a nice logo.

### How is Taekwindow licensed?

Taekwindow uses the BSD license. This is a very liberal license; in particular, you are free to distribute and modify the program. (Some websites got it wrong, and list Taekwindow under the GPL license instead.)

Technical
---------

### Does it run on Windows Vista and Windows 7?

As of version 0.3, these operating systems are fully supported. (Previous versions work reasonably as well.)

### Does it run on 64-bits Windows?

As of version 0.3, it does.

### Do I really need the .NET framework?

No. Version 0.2.x required the .NET framework for the configuration tool. As of version 0.3, .NET is no longer used at all.

### Can I run it from a USB stick?

Yes. The program has always been "portable" in the sense that it does not require installation. As of version 0.3.1, you can choose to store your settings in an INI file that you can put on your portable drive alongside the program.

### Will there ever be a Mac version?

I don't own or use a Mac, so I will not write this. Furthermore, Taekwindow is so closely tied to the Windows platform that porting it would come down to a complete rewrite.

If you are a Mac developer willing to undertake a port, contact me!

Usage
-----

### I hid the icon; how do I get it back?

Just run `taekwindow.exe` again. The Preferences dialog will pop up, where you can re-enable the icon or quit the program.

### Alt-dragging interferes with Photoshop!

Yes, it does. A customizable blacklist is in the works, so that exceptions can be made for Photoshop and other applications that need to handle Alt-drag themselves.

Until then, you can tell Taekwindow to use the left Alt key, and use the right one for Photoshop, or vice versa. Alternatively, you can temporarily disable Taekwindow by clicking its notification area icon.
