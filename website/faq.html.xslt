<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="website.xslt"/>
	<xsl:import href="../meta.xslt"/>
	<xsl:import href="../doc2html.xslt"/>

	<xsl:output omit-xml-declaration="yes"/>

	<xsl:variable name="faq" select="document('../doc/faq.xml')/faq"/>

	<xsl:variable name="title">FAQ</xsl:variable>

	<xsl:variable name="content">

		<h1>Frequently Asked Questions</h1>

		<ul>
			<xsl:for-each select="$faq/category">
				<xsl:element name="li">
					<xsl:value-of select="title"/>
					<ul>
						<xsl:for-each select="qa">
							<li>
								<xsl:element name="a">
									<xsl:attribute name="href">
										#<xsl:value-of select="@id"/>
									</xsl:attribute>
									<xsl:value-of select="question"/>
								</xsl:element>
							</li>
						</xsl:for-each>
					</ul>
				</xsl:element>

			</xsl:for-each>
		</ul>

		<xsl:for-each select="$faq/category">
			<xsl:element name="h2">
				<xsl:attribute name="id">
					<xsl:value-of select="@id"/>
				</xsl:attribute>
				<xsl:value-of select="title"/>
			</xsl:element>
			<xsl:for-each select="qa">
				<xsl:element name="h3">
					<xsl:attribute name="id">
						<xsl:value-of select="@id"/>
					</xsl:attribute>
					<xsl:apply-templates select="question"/>
				</xsl:element>
				<xsl:apply-templates select="answer"/>
			</xsl:for-each>
		</xsl:for-each>

	</xsl:variable>

</xsl:stylesheet>
