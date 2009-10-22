<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="metadata.xslt"/>
	<xsl:output method="text"/>
	
	<xsl:template match="information">
		/* This file is automatically generated from metadata-c.xslt and information.xml. Do not edit. */
		#define APPLICATION_TITLE             "<xsl:value-of select="apptitle"/>"
		#define APPLICATION_AUTHOR            "<xsl:value-of select="author"/>"
		#define APPLICATION_AUTHOR_FULL       "<xsl:call-template name="authorfull"/>"
		#define APPLICATION_COPYRIGHT         "<xsl:value-of select="copyright"/>"
		#define APPLICATION_VERSION           <xsl:call-template name="fullversion"><xsl:with-param name="separator">,</xsl:with-param></xsl:call-template>
		#define APPLICATION_VERSION_STRING    "<xsl:call-template name="fullversion"/>"
		#define APPLICATION_VERSION_FILENAME  <xsl:call-template name="shortversion"/>
		#define APPLICATION_WEBSITE           "<xsl:value-of select="website"/>"
		#define APPLICATION_EMAIL             "<xsl:value-of select="email"/>"
		#define APPLICATION_LICENSE_BRIEF     "<xsl:value-of select="licensebrief"/>"
		#define APPLICATION_README_FILE       "<xsl:value-of select="readmefile"/>"
		#define MAIN_EXE_FILE                 "<xsl:value-of select="mainexefile"/>"
		#define HOOKS_DLL_FILE                "<xsl:value-of select="hooksdllfile"/>"
	</xsl:template>

</xsl:stylesheet> 
