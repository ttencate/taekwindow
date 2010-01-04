<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="../meta.xslt"/>
	<xsl:import href="../doc2html.xslt"/>
	
	<xsl:output omit-xml-declaration="yes"/>

	<xsl:template match="/">
		<xsl:comment xml:space="preserve">#set var="title" value="Download"</xsl:comment>
		<xsl:comment xml:space="preserve">#set var="lastmod" value="$LAST_MODIFIED"</xsl:comment>
		<xsl:comment xml:space="preserve">#include virtual="header.part.shtml"</xsl:comment>

		<a href="http://www.softpedia.com/progClean/Taekwindow-Clean-70130.html">
			<img src="softpedia-free-award.gif" alt="100% FREE award granted by Softpedia" title="Softpedia guarantees that Taekwindow 0.2.4 is 100% Free, which means it does not contain any form of malware, including but not limited to: spyware, viruses, trojans and backdoors." class="rightmargin"/>
		</a>
		<a href="http://www.geardownload.com/desktop/taekwindow.html">
			<img src="geardownload-clean.gif" alt="Tested 100% clean on GearDownload.com" title="This download was tested thoroughly and was found 100% clean." class="rightmargin"/>
		</a>
		<a href="http://www.geardownload.com/desktop/taekwindow.html">
			<img src="geardownload-5stars.gif" alt="Rated 5 stars on GearDownload.com" title="Rated 5 stars on GearDownload.com" class="rightmargin"/>
		</a>

		<h1>Download <xsl:value-of select="$apptitle"/></h1>

		<p>This ZIP file contains the latest version of the program itself and other required files.</p>

		<p>
			<strong>
				<xsl:element name="a">
					<xsl:attribute name="href">
						<xsl:text xml:space="preserve">http://downloads.sourceforge.net/taekwindow/</xsl:text>
						<xsl:value-of select="$binaryzipfile"/>
					</xsl:attribute>
					<xsl:attribute name="class">sf</xsl:attribute>
					<xsl:text xml:space="preserve">Download </xsl:text>
					<xsl:value-of select="$apptitle"/>
					<xsl:text xml:space="preserve"> </xsl:text>
					<xsl:value-of select="$shortversion"/>
				</xsl:element>
			</strong>
		</p>

		<p>
			Note that this version of the program no longer needs the .NET Framework.
		</p>

		<h1>Source code</h1>

		It is possible to download the source code of the program, in the form of a Visual Studio 2008 solution. This is probably only interesting for developers.

		<p>
			<strong>
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
			</strong>
		</p>

		<h1>RSS feed</h1>

		<p>
			To stay updated with the latest version, you can subscribe to the <a href="http://sourceforge.net/export/rss2_projnews.php?group_id=185457&amp;rss_fulltext=1">Taekwindow News RSS Feed</a>.
		</p>

		<xsl:comment xml:space="preserve">#include virtual="footer.part.shtml"</xsl:comment>
	</xsl:template>

</xsl:stylesheet>
