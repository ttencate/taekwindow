<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="../meta.xslt"/>
	<xsl:import href="../doc2html.xslt"/>

	<xsl:output method="html" encoding="utf-8" indent="no"/>

	<xsl:template match="/">
		<html>
			<head>
				<title>
					<xsl:value-of select="$appnameversion"/> Readme
				</title>
			</head>
			<body>
				<h1>
					<xsl:value-of select="$appnameversion"/> Readme
				</h1>
				<xsl:apply-templates select="document('description.xml')/description"/>
				<xsl:apply-templates select="document('sysreqs.xml')/sysreqs"/>
				<xsl:apply-templates select="document('installation.xml')/installation"/>
				<xsl:apply-templates select="document('usage.xml')/usage"/>
				<xsl:apply-templates select="document('configuration.xml')/configuration"/>
				<xsl:apply-templates select="document('bugs.xml')/bugs"/>
				<xsl:apply-templates select="document('history.xml')/history"/>
				<xsl:apply-templates select="document('contact.xml')/contact"/>
				<xsl:apply-templates select="document('thanks.xml')/thanks"/>
				<xsl:apply-templates select="document('license.xml')/license"/>
			</body>
		</html>
	</xsl:template>

</xsl:stylesheet> 
