<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="website.xslt"/>
	<xsl:import href="../doc2html.xslt"/>
	<xsl:output omit-xml-declaration="yes"/>

	<xsl:variable name="title">User's Manual</xsl:variable>
	
	<xsl:variable name="content">
		
		<h1>User's Manual</h1>

		<xsl:apply-templates select="document('../doc/sysreqs.xml')/sysreqs"/>
		<xsl:apply-templates select="document('../doc/installation.xml')/installation"/>
		<xsl:apply-templates select="document('../doc/usage.xml')/usage"/>
		<xsl:apply-templates select="document('../doc/configuration.xml')/configuration"/>
		<xsl:apply-templates select="document('../doc/history.xml')/history"/>
		<xsl:apply-templates select="document('../doc/thanks.xml')/thanks"/>
		<xsl:apply-templates select="document('../doc/license.xml')/license"/>

	</xsl:variable>

</xsl:stylesheet>
