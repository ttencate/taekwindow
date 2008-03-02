<?xml version="1.0" encoding="utf-8"?>

	<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="metadata.xslt"/>
		<xsl:output method="text" omit-xml-declaration="yes"/>

	<!-- Gives a plain-text change log of the latest version, for copy/pasting into SourceForge -->
	<xsl:template match="information">
		<xsl:variable name="shortversion">
			<xsl:call-template name="shortversion"/>
		</xsl:variable>
		<xsl:for-each select="versions/version">
			<xsl:if test="@nr = $shortversion">
				<xsl:for-each select="feature">
					<xsl:text xml:space="preserve">- </xsl:text>
					<xsl:value-of select="normalize-space(.)"/>
					<xsl:text xml:space="preserve">
</xsl:text>
				</xsl:for-each>
				<xsl:for-each select="bugfix">
					<xsl:text xml:space="preserve">- Bugfix: </xsl:text>
					<xsl:value-of select="normalize-space(.)"/>
					<xsl:text xml:space="preserve">
</xsl:text>
				</xsl:for-each>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

</xsl:stylesheet> 
