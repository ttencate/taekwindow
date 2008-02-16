<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="metadata.xslt"/>
	<xsl:import href="doc2html.xslt"/>
	<xsl:output omit-xml-declaration="yes"/>

	<xsl:template match="information">
		<xsl:comment xml:space="preserve">#set var="title" value="Download"</xsl:comment>
		<xsl:comment xml:space="preserve">#set var="lastmod" value="$LAST_MODIFIED"</xsl:comment>
		<xsl:comment xml:space="preserve">#include virtual="header.part.shtml"</xsl:comment>
		
		<h1>Download <xsl:value-of select="apptitle"/></h1>

		<p>This ZIP file contains the latest version of the program itself and other required files.</p>

		<p>
			<strong>
				<xsl:element name="a">
					<xsl:attribute name="href">
						<xsl:text xml:space="preserve">http://downloads.sourceforge.net/taekwindow/</xsl:text>
						<xsl:call-template name="binaryzipfile"/>
					</xsl:attribute>
					<xsl:attribute name="class">sf</xsl:attribute>
					<xsl:text>Download Taekwindow</xsl:text>
					<xsl:call-template name="shortversion"/>
				</xsl:element>
			</strong>
		</p>

		<p>
			Note that the configuration tool requires the Microsoft .NET Framework, version 2.0 or above. It can be downloaded from the <a href="http://msdn2.microsoft.com/en-us/netframework/aa569263.aspx">Microsoft .NET Framework download page</a>.
		</p>

		<h1>Source code</h1>

		It is possible to download the source code of the program, in the form of a Visual Studio 2005 solution. This is probably only interesting for developers.

		<p>
			<strong>
				<xsl:element name="a">
					<xsl:attribute name="href">
						<xsl:text xml:space="preserve">http://downloads.sourceforge.net/taekwindow/</xsl:text>
						<xsl:call-template name="sourcezipfile"/>
					</xsl:attribute>
					<xsl:attribute name="class">sf</xsl:attribute>
					<xsl:text>Download Taekwindow</xsl:text>
					<xsl:call-template name="shortversion"/>
				</xsl:element>
			</strong>
		</p>


		<xsl:comment xml:space="preserve">#include virtual="footer.part.shtml"</xsl:comment>
	</xsl:template>

</xsl:stylesheet>
