<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="../meta.xslt"/>
	<xsl:import href="website.xslt"/>

	<xsl:variable name="title">Download</xsl:variable>

	<xsl:variable name="content">

		<p class="rightmargin awards">
			<a href="http://www.softpedia.com/progClean/Taekwindow-Clean-70130.html">
				<img src="softpedia-free-award.gif" alt="100% FREE award granted by Softpedia" title="Softpedia guarantees that Taekwindow 0.2.4 is 100% Free, which means it does not contain any form of malware, including but not limited to: spyware, viruses, trojans and backdoors."/>
			</a>
			<a href="http://www.geardownload.com/desktop/taekwindow.html">
				<img src="geardownload-clean.gif" alt="Tested 100% clean on GearDownload.com" title="This download was tested thoroughly and was found 100% clean."/>
			</a>
			<a href="http://www.geardownload.com/desktop/taekwindow.html">
				<img src="geardownload-5stars.gif" alt="Rated 5 stars on GearDownload.com" title="Rated 5 stars on GearDownload.com"/>
			</a>
			<a href="http://www.softoxi.com/taekwindow.html">
				<img src="softoxi-award.png" alt="Taekwindow antivirus scan report at softoxi.com" title="Taekwindow Antivirus Scan Report done by Softoxi.com"/>
			</a>
			<a href="http://www.softoxi.com/taekwindow-video-trailer-screenshots.html">
				<img src="softoxi-award_star.png" alt="Taekwindow video tutorial at softoxi.com" title="Taekwindow Video Tutorial done by Softoxi.com"/>
			</a>
		</p>

		<h1>
			Download <xsl:value-of select="$apptitle"/>
		</h1>

		<p>This ZIP file contains the latest version of the program itself.</p>

		<p>
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:text xml:space="preserve">http://downloads.sourceforge.net/taekwindow/</xsl:text>
					<xsl:value-of select="$binaryzipfile"/>
				</xsl:attribute>
				<xsl:attribute name="class">download</xsl:attribute>
				<img src="download.png" alt="Download icon" style="margin-bottom:-15px"/>
				<xsl:text xml:space="preserve">Download </xsl:text>
				<xsl:value-of select="$apptitle"/>
				<xsl:text xml:space="preserve"> </xsl:text>
				<xsl:value-of select="$shortversion"/>
			</xsl:element>
		</p>

		<h1>Like it? Click the button!</h1>

		<p>
			I work on this program in my spare time, and give it away to the world without demanding anything in return. However, if you want to show your appreciation and support, please consider donating. Through PayPal, it's quick and easy.
		</p>
		<p>
			<a href="http://sourceforge.net/donate/index.php?group_id=185457" class="donate">
				<img src="http://images.sourceforge.net/images/project-support.jpg" width="88" height="32" alt="Support this project"/>
				Send a donation
			</a>
		</p>

		<h1>Source code</h1>

		<p>
			It is possible to download the source code of the program, in the form of a Visual C++ 2008 solution. This is probably only interesting for developers.
		</p>

		<p>
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:text xml:space="preserve">http://downloads.sourceforge.net/taekwindow/</xsl:text>
					<xsl:value-of select="$sourcezipfile"/>
				</xsl:attribute>
				<xsl:attribute name="class">sf</xsl:attribute>
				<xsl:text xml:space="preserve">Download </xsl:text>
				<xsl:value-of select="$apptitle"/>
				<xsl:text xml:space="preserve"> </xsl:text>
				<xsl:value-of select="$shortversion"/>
				<xsl:text xml:space="preserve"> source code</xsl:text>
			</xsl:element>
		</p>

		<h1>RSS feed</h1>

		<p>
			To stay updated with the latest version, you can subscribe to the <a href="http://sourceforge.net/export/rss2_projnews.php?group_id=185457&amp;rss_fulltext=1">
				<img src="feed-icon-14x14.png" alt="RSS icon" class="inline"/> Taekwindow News RSS Feed
			</a>.
		</p>

	</xsl:variable>

</xsl:stylesheet>
