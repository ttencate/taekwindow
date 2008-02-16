<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="doc2html.xslt"/>

	<xsl:template match="documentation">
		<html>
			<head>
				<title>
					<xsl:value-of select="@app"/><xsl:text xml:space="preserve"> </xsl:text><xsl:value-of select="@version"/> Readme
				</title>
			</head>
			<body>
				<h1>
					<xsl:value-of select="@app"/><xsl:text xml:space="preserve"> </xsl:text><xsl:value-of select="@version"/> Readme
				</h1>
				<xsl:apply-templates/>
			</body>
		</html>
	</xsl:template>

</xsl:stylesheet> 
