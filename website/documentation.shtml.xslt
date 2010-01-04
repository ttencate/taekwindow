<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="../doc2html.xslt"/>
	<xsl:output omit-xml-declaration="yes"/>

	<xsl:template match="/">
		<xsl:comment xml:space="preserve">#set var="title" value="Documentation"</xsl:comment>
		<xsl:comment xml:space="preserve">#set var="lastmod" value="$LAST_MODIFIED"</xsl:comment>
		<xsl:comment xml:space="preserve">#include virtual="header.part.shtml"</xsl:comment>
		
		<h1>Documentation</h1>

		<xsl:apply-templates select="document('../doc/sysreqs.xml')/sysreqs"/>
		<xsl:apply-templates select="document('../doc/installation.xml')/installation"/>
		<xsl:apply-templates select="document('../doc/usage.xml')/usage"/>
		<xsl:apply-templates select="document('../doc/configuration.xml')/configuration"/>
		<xsl:apply-templates select="document('../doc/history.xml')/history"/>
		<xsl:apply-templates select="document('../doc/thanks.xml')/thanks"/>
		<xsl:apply-templates select="document('../doc/license.xml')/license"/>

		<xsl:comment xml:space="preserve">#include virtual="footer.part.shtml"</xsl:comment>
	</xsl:template>

</xsl:stylesheet>
