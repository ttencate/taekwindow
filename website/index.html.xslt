<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="../meta.xslt"/>
	<xsl:import href="website.xslt"/>

	<xsl:variable name="contact" select="document('../doc/contact.xml')/contact"/>

	<xsl:variable name="title">Home</xsl:variable>

	<xsl:variable name="content">

		<p class="rightmargin">
			<img src="preferences.png" alt="A screenshot of the Taekwindow preferences dialog"/><br/>
			The Taekwindow preferences dialog
		</p>
		
		<h1>What is Taekwindow?</h1>
		<p>Taekwindow is a simple, lightweight and free Windows program that allows you to move and resize windows by dragging them with the Alt key, similar to many X11 window managers.</p>

		<h1>Why would I want it?</h1>
		<p>If you're a Linux or other Unix user, you are probably familiar with Alt-dragging. Once you have gotten used to it, you'll want to use it in Windows as well.</p>
		<p>But even if you're not used to Alt-dragging, it can still make your Windows experience more pleasant. Especially when you have a large monitor, or multiple monitors, you're moving your mouse much more than you ought to. Taekwindow can save you time, frustration and RSI!</p>
		
		<h1>What exactly does it do?</h1>
		<p>In full detail, Taekwindow allows you to do the following:</p>
		<ul>
			<li>
				move a window by grabbing it <em>anywhere</em> (not just the title bar) while holding the Alt key, and then dragging with the left mouse button;
			</li>
			<li>
				resize a window by grabbing it <em>anywhere</em> (not just the tiny little border) while holding the Alt key, and then dragging with the right mouse button;
			</li>
			<li>move a maximized window between monitors by Alt-dragging;</li>
			<li>use the scroll wheel on the window under the cursor, instead of the currently focused window;</li>
			<li>push a window to the background by middle-clicking on its title bar.</li>
		</ul>
		<p>All these features can be configured to your liking.</p>

		<h1>I'm convinced. How do I get it?</h1>
		<p>
			Taekwindow is completely free. It is a tiny download, and does not require installation: you can be running it only seconds from now. <strong>
				<a href="download.shtml">Go and download it!</a>
			</strong>
		</p>
		
	</xsl:variable>

</xsl:stylesheet>
