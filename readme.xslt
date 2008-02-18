<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="metadata.xslt"/>
	<xsl:import href="doc2html.xslt"/>

	<xsl:template match="information">
		<html>
			<head>
				<title>
					<xsl:call-template name="appnameversion"/> Readme
				</title>
			</head>
			<body>
				<h1>
					<xsl:call-template name="appnameversion"/> Readme
				</h1>
				<xsl:apply-templates select="description"/>
				<xsl:apply-templates select="sysreqs"/>
				<xsl:apply-templates select="installation"/>
				<xsl:apply-templates select="usage"/>
				<xsl:apply-templates select="configuration"/>
				<xsl:apply-templates select="bugs"/>
				<xsl:apply-templates select="feature-requests"/>
				<xsl:apply-templates select="versions"/>
				<xsl:apply-templates select="contact"/>
				<xsl:apply-templates select="thanks"/>
				<xsl:apply-templates select="license"/>
			</body>
		</html>
	</xsl:template>

</xsl:stylesheet> 
