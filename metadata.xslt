<?xml version="1.0" encoding="utf-8"?>

	<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template name="fullversion">
		<xsl:param name="separator">.</xsl:param>
		<xsl:value-of select="majorversion"/>
		<xsl:value-of select="$separator"/>
		<xsl:value-of select="minorversion"/>
		<xsl:value-of select="$separator"/>
		<xsl:value-of select="revision"/>
		<xsl:value-of select="$separator"/>
		<xsl:value-of select="build"/>
	</xsl:template>

	<xsl:template name="shortversion">
		<xsl:param name="separator">.</xsl:param>
		<xsl:value-of select="majorversion"/>
		<xsl:value-of select="$separator"/>
		<xsl:value-of select="minorversion"/>
		<xsl:if test="revision != '0' or build != '0'">
			<xsl:value-of select="$separator"/>
			<xsl:value-of select="revision"/>
			<xsl:if test="build != '0'">
				<xsl:value-of select="$separator"/>
				<xsl:value-of select="build"/>
			</xsl:if>
		</xsl:if>
	</xsl:template>

	<xsl:template name="appnameversion">
		<xsl:value-of select="apptitle"/>
		<xsl:text xml:space="preserve"> </xsl:text>
		<xsl:call-template name="shortversion"/>
	</xsl:template>

	<xsl:template name="authorfull">
		<xsl:value-of select="author"/>
		<xsl:text xml:space="preserve"> &lt;</xsl:text>
		<xsl:value-of select="email"/>
		<xsl:text xml:space="preserve">&gt;</xsl:text>
	</xsl:template>

	<xsl:template name="binaryzipfile">
		<xsl:value-of select="filename"/>
		<xsl:text xml:space="preserve">-</xsl:text>
		<xsl:call-template name="shortversion"/>
		<xsl:text xml:space="preserve">.zip</xsl:text>
	</xsl:template>

	<xsl:template name="sourcezipfile">
		<xsl:value-of select="filename"/>
		<xsl:text xml:space="preserve">-</xsl:text>
		<xsl:call-template name="shortversion"/>
		<xsl:text xml:space="preserve">-src.zip</xsl:text>
	</xsl:template>

</xsl:stylesheet> 
