<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="../meta.xslt"/>
	<xsl:import href="../doc2html.xslt"/>

	<xsl:output omit-xml-declaration="yes"/>

	<xsl:variable name="faq" select="document('../doc/faq.xml')/faq"/>

	<xsl:template match="/">
		<xsl:comment xml:space="preserve">#set var="title" value="Download"</xsl:comment>
		<xsl:comment xml:space="preserve">#set var="lastmod" value="$LAST_MODIFIED"</xsl:comment>
		<xsl:comment xml:space="preserve">#include virtual="header.part.shtml"</xsl:comment>

		<h1>Frequently Asked Questions</h1>

		<ul>
			<xsl:for-each select="$faq/category">
				<xsl:element name="li">
					<xsl:attribute name="id">
						<xsl:value-of select="@id"/>
					</xsl:attribute>
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

		<xsl:comment xml:space="preserve">#include virtual="footer.part.shtml"</xsl:comment>
	</xsl:template>

</xsl:stylesheet>
