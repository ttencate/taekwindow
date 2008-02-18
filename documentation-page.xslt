<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="doc2html.xslt"/>
	<xsl:output omit-xml-declaration="yes"/>

	<xsl:template match="information">
		<xsl:comment xml:space="preserve">#set var="title" value="Documentation"</xsl:comment>
		<xsl:comment xml:space="preserve">#set var="lastmod" value="$LAST_MODIFIED"</xsl:comment>
		<xsl:comment xml:space="preserve">#include virtual="header.part.shtml"</xsl:comment>
		
		<h1>Documentation</h1>

		<xsl:apply-templates select="sysreqs"/>
		<xsl:apply-templates select="installation"/>
		<xsl:apply-templates select="usage"/>
		<xsl:apply-templates select="configuration"/>
		<xsl:apply-templates select="versions"/>
		<xsl:apply-templates select="thanks"/>
		<xsl:apply-templates select="license"/>

		<xsl:comment xml:space="preserve">#include virtual="footer.part.shtml"</xsl:comment>
	</xsl:template>

</xsl:stylesheet>
