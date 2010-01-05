<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="website.xslt"/>
	<xsl:import href="../meta.xslt"/>
	<xsl:import href="../doc2html.xslt"/>

	<xsl:variable name="contact" select="document('../doc/contact.xml')/contact"/>

	<xsl:variable name="title">Contact</xsl:variable>

	<xsl:variable name="content">
		
		<h1>Contact</h1>

		<xsl:apply-templates select="$contact/*"/>
		
	</xsl:variable>

</xsl:stylesheet>
